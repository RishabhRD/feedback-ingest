#pragma once

#include "data_source/types.hpp"
#include "schema.hpp"
#include <functional>
#include <nlohmann/json.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string>
#include <vector>

namespace rd {
namespace notifyu {

inline auto parse_schema(source_id_t source_id, tenant_id_t tenant_id,
                         nlohmann::json const &json) {
  rd::metadata_t metadata{
      .location = json["country"],
      .app_version = json["app_version"],
      .rating = json["rating"],
      .impressions = json["likes"],
  };

  rd::review review = json["review"];

  rd::data_source_entry_t entry{
      .metadata = std::move(metadata),
      .feedback = std::move(review),
  };

  return rd::schema_t{
      .source_id = source_id,
      .tenant_id = tenant_id,
      .entry = std::move(entry),
  };
}

inline auto parse_schema_list(source_id_t source_id, tenant_id_t tenant_id,
                              nlohmann::json const &json) {
  namespace rv = ranges::views;
  return json                                                                 //
         | rv::transform(std::bind_front(parse_schema, source_id, tenant_id)) //
         | ranges::to_vector;
}

std::pair<std::string, std::vector<rd::schema_t>> inline parse_content(
    source_id_t source_id, tenant_id_t tenant_id, std::string_view content) {
  auto json = nlohmann::json::parse(content);
  std::string secret_token = json["secret_token"];
  auto schemas = parse_schema_list(source_id, tenant_id, json["review_stream"]);
  return {std::move(secret_token), std::move(schemas)};
}

inline bool is_good_secret_token(std::string_view token) {
  return token == "12345678";
}

} // namespace notifyu
} // namespace rd
