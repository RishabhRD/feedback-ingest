#include "schema.hpp"
#include "schema_dump/json_schema_dump.hpp"
#include <doctest/doctest.h>

TEST_CASE("creates correct file name") {
  rd::schema_t schema;
  schema.metadata = {
      .id = "playstore_enterpret.txt",
      .source = "File",
      .country = "India",
      .app_version = tl::nullopt,
  };
  schema.feedback = rd::review{"A great app, must use!"};
  REQUIRE_EQ(rd::create_json_file_name(".", schema),
             "./File_playstore_enterpret.txt.json");
}
