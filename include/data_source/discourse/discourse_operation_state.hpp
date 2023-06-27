#pragma once

#include "algorithms.hpp"
#include "coro.hpp"
#include "data_source/data_source_info_registry.hpp"
#include "data_source/discourse/discourse_info.hpp"
#include "data_source/types.hpp"
#include "http_ops.hpp"
#include "schema.hpp"
#include "sink/sink_concepts.hpp"
#include "tenant/tenant_registry.hpp"
#include <chrono>
#include <functional>
#include <spdlog/spdlog.h>

namespace rd {
namespace discourse {
using namespace std::chrono_literals;
inline rd::awaitable<std::vector<rd::schema_t>>
fetch_posts_between(int source_id, int tenant_id, int topic_id,
                    std::chrono::system_clock::time_point begin,
                    std::chrono::system_clock::time_point end) {
  auto range_url = make_url_for_posts_between(begin, end);
  auto range_url_res = co_await rd::http_get(range_url);
  auto post_ids = get_post_ids(range_url_res, topic_id);
  auto post_ids_url = make_url_for_post_ids(topic_id, post_ids);
  auto post_ids_url_res = co_await rd::http_get(post_ids_url);
  co_return raw_posts_to_schema(post_ids_url_res, source_id, tenant_id);
}

template <typename Timer, rd::is_sink Sink> struct discourse_operation_state_t {

  discourse_operation_state_t(source_id_t source_id_, tenant_id_t tenant_id_,
                              discourse_info_t discourse_info_,
                              Timer schedule_every_,
                              std::reference_wrapper<Sink> sink_)
      : source_id(source_id_), tenant_id(tenant_id_),
        discourse_info(discourse_info_),
        schedule_every(std::move(schedule_every_)), sink(std::move(sink_)) {}

  discourse_operation_state_t(discourse_operation_state_t const &) = delete;
  discourse_operation_state_t(discourse_operation_state_t &&) = default;

  rd::awaitable<void> start() {
    try {
      auto op = [&](auto prev_time) {
        return extract_transform_and_load(std::move(prev_time));
      };
      co_await schedule_every(
          std::chrono::sys_days{2023y / 1 / 1}, std::chrono::days(1), op,
          std::to_string(tenant_id) + "_" + std::to_string(source_id));
    } catch (std::exception &e) {
      spdlog::error("Discourse source crashed");
      spdlog::error("source_id: {}", source_id);
      spdlog::error("tenant_id: {}", tenant_id);
      spdlog::error("exception info: {}", e.what());
    }
  }

private:
  auto extract_transform_and_load(auto prev_time) -> rd::awaitable<void> {
    spdlog::info("Discourse: Processing data for source_id: {}, tenant_id: {}",
                 source_id, tenant_id);
    auto schemas = co_await fetch_posts_between(
        source_id, tenant_id, discourse_info.topic_id, prev_time,
        std::chrono::system_clock::now());
    co_await sink.get().load(schemas);
  }

  source_id_t source_id;
  tenant_id_t tenant_id;
  discourse_info_t discourse_info;
  Timer schedule_every;
  std::reference_wrapper<Sink> sink;
};

} // namespace discourse
} // namespace rd
