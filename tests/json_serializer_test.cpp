#include "optional.hpp"
#include "schema.hpp"
#include "schema_serializers/json_serializer.hpp"
#include <doctest/doctest.h>
#include <string>

using json = nlohmann::json;

TEST_CASE("Can serialize conversation") {
  rd::data_source_entry_t entry;
  entry.metadata = {
      .location = "India",
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = 20000,
  };
  entry.feedback = rd::conversation{
      {rd::user_1{}, "Hello"},
      {rd::user_1{}, "Can you Help me"},
      {rd::user_2{}, "Yes How can I help you?"},
  };

  rd::schema_t schema{
      .source_id = 123,
      .tenant_id = 321,
      .entry = entry,
  };

  json expected = {
      {"source_id", 123},
      {"tenant_id", 321},
      {"entry",
       {{"metadata",
         {
             {"location", "India"},
             {"app_version", "4.1.2"},
             {"rating", 5},
             {"impressions", 20000},
         }},
        {"feedback",
         {{"feedback_type", "conversation"},
          {"data",
           {
               {{"user", "user1"}, {"message", "Hello"}},
               {{"user", "user1"}, {"message", "Can you Help me"}},
               {{"user", "user2"}, {"message", "Yes How can I help you?"}},
           }}}}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}

TEST_CASE("Can serialize review") {
  rd::data_source_entry_t entry;
  entry.metadata = {
      .location = "India",
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = 20000,
  };
  entry.feedback = "A great app, must try!";

  rd::schema_t schema{
      .source_id = 123,
      .tenant_id = 321,
      .entry = entry,
  };

  json expected = {
      {"source_id", 123},
      {"tenant_id", 321},
      {"entry",
       {{"metadata",
         {
             {"location", "India"},
             {"app_version", "4.1.2"},
             {"rating", 5},
             {"impressions", 20000},
         }},
        {"feedback",
         {
             {"feedback_type", "review"},
             {"data", "A great app, must try!"},
         }}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}

TEST_CASE("Is skipping null field") {
  rd::data_source_entry_t entry;
  entry.metadata = {
      .location = tl::nullopt,
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = tl::nullopt,
  };
  entry.feedback = "A great app, must try!";

  rd::schema_t schema{
      .source_id = 123,
      .tenant_id = 321,
      .entry = entry,
  };

  json expected = {
      {"source_id", 123},
      {"tenant_id", 321},
      {"entry",
       {{"metadata",
         {
             {"app_version", "4.1.2"},
             {"rating", 5},
         }},
        {"feedback",
         {
             {"feedback_type", "review"},
             {"data", "A great app, must try!"},
         }}}},
  };

  REQUIRE_EQ(rd::serialize_to_json(schema), expected);
}

TEST_CASE("Can serialize multiple schemas") {
  rd::data_source_entry_t entry1;
  entry1.metadata = {
      .location = tl::nullopt,
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = tl::nullopt,
  };
  entry1.feedback = "A great app, must try!";

  rd::schema_t schema1{
      .source_id = 123,
      .tenant_id = 321,
      .entry = entry1,
  };

  rd::data_source_entry_t entry2;
  entry2.metadata = {
      .location = tl::nullopt,
      .app_version = "4.1.3",
      .rating = 6,
      .impressions = tl::nullopt,
  };
  entry2.feedback = "A great app, must try!";

  rd::schema_t schema2{
      .source_id = 124,
      .tenant_id = 421,
      .entry = entry2,
  };

  json expected = {{
                       {"source_id", 123},
                       {"tenant_id", 321},
                       {"entry",
                        {{"metadata",
                          {
                              {"app_version", "4.1.2"},
                              {"rating", 5.0},
                          }},
                         {"feedback",
                          {
                              {"feedback_type", "review"},
                              {"data", "A great app, must try!"},
                          }}}},
                   },
                   {
                       {"source_id", 124},
                       {"tenant_id", 421},
                       {"entry",
                        {{"metadata",
                          {
                              {"app_version", "4.1.3"},
                              {"rating", 6.0},
                          }},
                         {"feedback",
                          {
                              {"feedback_type", "review"},
                              {"data", "A great app, must try!"},
                          }}}},
                   }};

  REQUIRE_EQ(rd::serialize_to_json(std::vector{schema1, schema2}), expected);
}
