#include "schema.hpp"
#include "schema_dump/json_schema_dump.hpp"
#include <doctest/doctest.h>

TEST_CASE("creates correct file name") {
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
  auto time_point = std::chrono::system_clock::now();
  REQUIRE_EQ(rd::create_json_file_name(".", time_point, schema),
             "./321/123_" +
                 std::to_string(time_point.time_since_epoch().count()) +
                 ".json");
}
