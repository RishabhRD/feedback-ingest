#pragma once

#include "data_source/types.hpp"
#include "optional.hpp"
#include "schema.hpp"
#include <functional>
#include <nlohmann/json.hpp>
#include <range/v3/range/conversion.hpp>
#include <range/v3/view/transform.hpp>
#include <string_view>
#include <vector>
namespace rd {
namespace filepp {

inline rd::conversation_entry_t
get_conversation_entry(nlohmann::json const &json) {
  if (json.contains("User")) {
    return {
        .user = rd::user_1{},
        .message = json["User"],
    };
  } else {
    return {
        .user = rd::user_2{},
        .message = json["Support"],
    };
  }
}

inline rd::conversation get_conversation(nlohmann::json const &json) {
  return json                                               //
         | ranges::views::transform(get_conversation_entry) //
         | ranges::to_vector;
}

inline rd::schema_t entry_to_schema(source_id_t source_id,
                                    tenant_id_t tenant_id,
                                    nlohmann::json const &json) {
  rd::metadata_t metadata{
      .location = tl::nullopt,
      .app_version = json["app_version"],
      .rating = tl::nullopt,
      .impressions = tl::nullopt,
  };
  auto conversation = get_conversation(json["conversation"]);
  rd::data_source_entry_t feedback{
      .metadata = std::move(metadata),
      .feedback = std::move(conversation),
  };
  return {
      .source_id = source_id,
      .tenant_id = tenant_id,
      .entry = std::move(feedback),
  };
}

inline std::vector<rd::schema_t> to_schema(source_id_t source_id,
                                           tenant_id_t tenant_id,
                                           std::string_view content) {
  auto json = nlohmann::json::parse(content);
  return ranges::views::transform(
             json, std::bind_front(entry_to_schema, source_id, tenant_id)) //
         | ranges::to_vector;
}
} // namespace filepp
} // namespace rd
