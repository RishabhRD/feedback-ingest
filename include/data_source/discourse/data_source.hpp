#pragma once

#include "algorithms.hpp"
#include "coro.hpp"
#include "http_ops.hpp"
#include "schema.hpp"
namespace rd {
namespace discourse {

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
} // namespace discourse
} // namespace rd
