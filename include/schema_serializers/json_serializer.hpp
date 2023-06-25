#pragma once

#include "../overload.hpp"
#include "../schema.hpp"
#include <boost/hof.hpp>
#include <nlohmann/json.hpp>
#include <variant>

namespace rd {
using json = nlohmann::json;
namespace __detail {
inline auto serialize_metadata(rd::metadata_t const &metadata) {
  json j;
  if (metadata.location)
    j["location"] = metadata.location.value();
  if (metadata.app_version)
    j["app_version"] = metadata.app_version.value();
  if (metadata.rating)
    j["rating"] = metadata.rating.value();
  if (metadata.impressions)
    j["impressions"] = metadata.impressions.value();
  return j;
}

inline auto
serialize_conversation_entry(rd::conversation_entry_t const &entry) {
  json j;
  j["message"] = entry.message;
  if (entry.user.index() == 0)
    j["user"] = "user1";
  else
    j["user"] = "user2";
  return j;
}

inline auto serialize_converstion(rd::conversation const &conversation) {
  json j;
  j["feedback_type"] = "conversation";
  for (auto const &conversation_entry : conversation) {
    j["data"].push_back(serialize_conversation_entry(conversation_entry));
  }
  return j;
}

inline auto serialize_review(rd::review const &review) {
  json j;
  j["feedback_type"] = "review";
  j["data"] = review;
  return j;
}

inline auto serialize_feedback(rd::feedback_t const &feedback) {
  json j;
  j["source_id"] = feedback.source_id;
  j["tenant_id"] = feedback.tenant_id;
  j["feedback_data"] = std::visit(
      rd::overloaded{
          BOOST_HOF_LIFT(serialize_converstion),
          BOOST_HOF_LIFT(serialize_review),
      },
      feedback.feedback_data);
  return j;
}
} // namespace __detail

inline auto serialize_to_json(rd::schema_t const &schema) {
  json j;
  j["metadata"] = __detail::serialize_metadata(schema.metadata);
  j["feedback"] = __detail::serialize_feedback(schema.feedback);
  return j;
}
} // namespace rd
