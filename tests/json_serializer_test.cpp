#include "optional.hpp"
#include "schema.hpp"
#include "schema_serializers/json_serializer.hpp"
#include <doctest/doctest.h>
#include <string>

using json = nlohmann::json;

TEST_CASE("Can serialize conversation") {
  rd::schema_t schema;
  schema.metadata = {
      .location = "India",
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = 20000,
  };
  schema.feedback = {
      .source_id = "some_special_source_id",
      .tenant_id = "some_special_tenant_id",
      .feedback_data =
          rd::conversation{
              {rd::user_1{}, "Hello"},
              {rd::user_1{}, "Can you Help me"},
              {rd::user_2{}, "Yes How can I help you?"},
          },
  };

  json expected = {
      {"metadata",
       json{
           {"location", "India"},
           {"app_version", "4.1.2"},
           {"rating", 5},
           {"impressions", 20000},
       }},
      {"feedback",
       {{"source_id", "some_special_source_id"},
        {"tenant_id", "some_special_tenant_id"},
        {"feedback_data",
         {{"feedback_type", "conversation"},
          {"data",
           json{
               json{{"user", "user1"}, {"message", "Hello"}},
               json{{"user", "user1"}, {"message", "Can you Help me"}},
               json{{"user", "user2"}, {"message", "Yes How can I help you?"}},
           }}}}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}

TEST_CASE("Can serialize review") {
  rd::schema_t schema;
  schema.metadata = {
      .location = "India",
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = 20000,
  };
  schema.feedback = {
      .source_id = "some_special_source_id",
      .tenant_id = "some_special_tenant_id",
      .feedback_data = rd::review{"A good product, must use!"},
  };

  json expected = {
      {"metadata",
       json{
           {"location", "India"},
           {"app_version", "4.1.2"},
           {"rating", 5},
           {"impressions", 20000},
       }},
      {"feedback",
       {{"source_id", "some_special_source_id"},
        {"tenant_id", "some_special_tenant_id"},
        {"feedback_data",
         {{"feedback_type", "review"},
          {"data", "A good product, must use!"}}}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}

TEST_CASE("Is skipping null field") {
  rd::schema_t schema;
  schema.metadata = {
      .location = tl::nullopt,
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = tl::nullopt,
  };
  schema.feedback = {
      .source_id = "some_special_source_id",
      .tenant_id = "some_special_tenant_id",
      .feedback_data = rd::review{"A good product, must use!"},
  };

  json expected = {
      {"metadata",
       json{
           {"app_version", "4.1.2"},
           {"rating", 5},
       }},
      {"feedback",
       {{"source_id", "some_special_source_id"},
        {"tenant_id", "some_special_tenant_id"},
        {"feedback_data",
         {{"feedback_type", "review"},
          {"data", "A good product, must use!"}}}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}
