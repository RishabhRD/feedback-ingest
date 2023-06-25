#include "schema.hpp"
#include "schema_dump/json_schema_dump.hpp"
#include <doctest/doctest.h>

TEST_CASE("creates correct file name") {
  rd::schema_t schema;
  schema.metadata = {
      .location = "India",
      .app_version = "4.1.2",
      .rating = 5,
      .impressions = 20000,
  };
  schema.feedback = {
      .source_id = "source_id",
      .tenant_id = "tenant_id",
      .feedback_data = rd::review{"A good product, must use!"},
  };
  auto time_point = std::chrono::system_clock::now();
  REQUIRE_EQ(rd::create_json_file_name(".", time_point, schema),
             "./tenant_id/source_id_" +
                 std::to_string(time_point.time_since_epoch().count()) +
                 ".json");
}
