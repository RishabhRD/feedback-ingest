#pragma once

#include "algorithms.hpp"
#include "coro.hpp"
#include "data_source/data_source_info_registry.hpp"
#include "data_source/discourse/discourse_info.hpp"
#include "data_source/types.hpp"
#include "http_ops.hpp"
#include "new_schema_handler.hpp"
#include "schema.hpp"
#include "tenant/tenant_registry.hpp"
#include <iostream>

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

  discourse_operation_state_t(source_id_t source_id_, tenant_id_t tenant_id_,
                              discourse_info_t discourse_info_,
                              Timer schedule_every_)
      : source_id(source_id_), tenant_id(tenant_id_),
        discourse_info(discourse_info_),
        schedule_every(std::move(schedule_every_)) {}

  discourse_operation_state_t(discourse_operation_state_t const &) = delete;
  discourse_operation_state_t(discourse_operation_state_t &&) = default;

  rd::awaitable<void> start() {
    try {
      auto extract_transform_load = [&](auto prev_time) -> rd::awaitable<void> {
        auto schemas = co_await fetch_posts_between(
            source_id, tenant_id, discourse_info.topic_id, prev_time,
            std::chrono::system_clock::now());
        co_await rd::on_new_schema_creation(schemas);
      };
      co_await schedule_every(std::chrono::sys_days{2023y / 1 / 1},
                              std::chrono::days(1), extract_transform_load,
                              std::to_string(tenant_id) + "_" +
                                  std::to_string(source_id));
    } catch (std::exception &e) {
      std::cout << e.what() << std::endl;
      std::cout << "discourse source crashed \nsource_id: " << source_id
                << "\ntenant_id: " << tenant_id << '\n'
                << e.what() << std::endl;
    }
  }

private:
  source_id_t source_id;
  tenant_id_t tenant_id;
  discourse_info_t discourse_info;
  Timer schedule_every;
};
template <typename Timer>
discourse_operation_state_t(
    typename discourse_operation_state_t<Timer>::source_id_t,
    typename discourse_operation_state_t<Timer>::tenant_id_t, discourse_info_t,
    Timer) -> discourse_operation_state_t<Timer>;

} // namespace discourse
} // namespace rd
