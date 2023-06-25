#pragma once

#include "coro.hpp"
#include "http_ops.hpp"
#include "optional.hpp"
#include "schema.hpp"
#include <chrono>
#include <fmt/chrono.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <iostream>
#include <nlohmann/json.hpp>
#include <range/v3/all.hpp>
#include <range/v3/range/conversion.hpp>

namespace rd {
namespace discourse {
using timepoint = std::chrono::system_clock::time_point;

inline auto
make_url_for_posts_between(std::chrono::system_clock::time_point begin,
                           std::chrono::system_clock::time_point end) {
  return fmt::format("https://meta.discourse.org/"
                     "search.json?q=after%3a{:%Y-%m-%d}+before%3a{:%Y-%m-%d}",
                     begin, end);
}

inline auto make_url_for_post_ids(int topic_id, std::vector<int> const &ids) {
  return fmt::format("https://meta.discourse.org/t/{:d}/"
                     "posts.json?post_ids%5B%5D={}",
                     topic_id, fmt::join(ids, ","));
}

namespace __deatil {

inline auto get_all_posts(std::string_view str) {
  using json = nlohmann::json;
  auto j = json::parse(str);
  return j["posts"];
}
} // namespace __deatil

inline auto get_post_ids(std::string_view str, int topic_id) {
  auto is_required_topic = [topic_id](auto const &post) {
    return post["topic_id"] == topic_id;
  };
  auto get_post_id = [](auto const &post) { return post["id"]; };
  auto posts = __deatil::get_all_posts(str);
  return posts                                      //
         | ranges::views::filter(is_required_topic) //
         | ranges::views::transform(get_post_id)    //
         | ranges::to<std::vector<int>>();
}

inline rd::data_source_entry_t
post_to_data_source_entry(nlohmann::json const &post) {
  double const rating = post["score"];
  int const impressions = post["readers_count"];
  std::string const review = post["cooked"];
  rd::metadata_t metadata = {
      .location = tl::nullopt,
      .app_version = tl::nullopt,
      .rating = rating,
      .impressions = impressions,
  };

  return {
      .metadata = metadata,
      .feedback = review,
  };
}

inline std::vector<rd::schema_t>
raw_posts_to_schema(std::string_view str, int source_id, int tenant_id) {
  auto to_schema = [&](rd::data_source_entry_t entry) -> rd::schema_t {
    return {
        .source_id = source_id,
        .tenant_id = tenant_id,
        .entry = std::move(entry),
    };
  };
  auto json = nlohmann::json::parse(str);
  auto posts = json["post_stream"]["posts"];
  return posts                                                 //
         | ranges::views::transform(post_to_data_source_entry) //
         | ranges::views::transform(to_schema)                 //
         | ranges::to<std::vector<rd::schema_t>>();
}

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

struct discourse_data_source_t {
  int topic_id;
};

} // namespace discourse

} // namespace rd
