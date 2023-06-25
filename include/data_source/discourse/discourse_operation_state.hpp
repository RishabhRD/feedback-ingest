#pragma once

#include "algorithms.hpp"
#include "coro.hpp"
#include "data_source/data_source_info_registry.hpp"
#include "data_source/discourse/discourse_info.hpp"
#include "http_ops.hpp"
#include "new_schema_handler.hpp"
#include "schema.hpp"
#include "tenant/tenant_registry.hpp"

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

template <typename Timer> struct discourse_operation_state_t {
  using tenant_id_t = rd::tenant_registry_t::key_t;
  using source_id_t = rd::data_source_info_registry_t::key_t;

  discourse_operation_state_t(discourse_operation_state_t const &) = delete;

  rd::awaitable<void> start() {
    auto extract_transform_load = [&](auto prev_time) -> rd::awaitable<void> {
      auto schemas = co_await fetch_posts_between(
          source_id, tenant_id, discourse_info.topic_id, prev_time,
          std::chrono::system_clock::now());
      co_await rd::on_new_schema_creation(schemas);
    };
    static_assert(rd::is_awaitable<decltype(extract_transform_load)>);
    co_await schedule_every(
        std::chrono::sys_days{2023y / 1 / 1}, 1d, extract_transform_load,
        std::to_string(tenant_id) + "_" + std::to_string(source_id));
  }

private:
  tenant_id_t tenant_id;
  source_id_t source_id;
  discourse_info_t discourse_info;
  Timer schedule_every;
};

} // namespace discourse
} // namespace rd
