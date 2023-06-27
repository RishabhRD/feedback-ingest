#include "schema.hpp"
#include "sink/json_sink.hpp"
#include <doctest/doctest.h>

TEST_CASE("creates correct file name") {
  auto time_point = std::chrono::system_clock::now();
  REQUIRE_EQ(rd::create_json_file_name(".", time_point, 123, 321),
             "./321/123_" +
                 std::to_string(time_point.time_since_epoch().count()) +
                 ".json");
}
