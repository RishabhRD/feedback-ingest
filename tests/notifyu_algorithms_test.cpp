#include "data_source/notifyu/algorithms.hpp"
#include <doctest/doctest.h>

TEST_CASE("Parsing the content in right way") {
  std::string content =
      R"({
  "secret_token": "12345678",
  "review_stream": [
    {
      "app_version": "4.18.9",
      "country": "India",
      "rating": 4.3,
      "likes": 28,
      "review": "Nice App, needs a dark theme"
    },
    {
      "app_version": "4.18.8",
      "country": "Bangladesh",
      "rating": 4.8,
      "likes": 1,
      "review": "Good app"
    }
  ]
})";

  auto const result = rd::notifyu::parse_content(123, 321, content);

  REQUIRE_EQ(result.first, "12345678");
  REQUIRE_EQ(result.second.size(), 2);

  REQUIRE_EQ(result.second[0].source_id, 123);
  REQUIRE_EQ(result.second[0].tenant_id, 321);
  REQUIRE_EQ(result.second[0].entry.metadata.app_version, "4.18.9");
  REQUIRE_EQ(result.second[0].entry.metadata.impressions, 28);
  REQUIRE_EQ(result.second[0].entry.metadata.location, "India");
  REQUIRE_EQ(result.second[0].entry.metadata.rating, 4.3);
  REQUIRE_EQ(result.second[0].entry.feedback.index(), 1);
  REQUIRE_EQ(std::get<std::string>(result.second[0].entry.feedback),
             "Nice App, needs a dark theme");

  REQUIRE_EQ(result.second[1].source_id, 123);
  REQUIRE_EQ(result.second[1].tenant_id, 321);
  REQUIRE_EQ(result.second[1].entry.metadata.app_version, "4.18.8");
  REQUIRE_EQ(result.second[1].entry.metadata.impressions, 1);
  REQUIRE_EQ(result.second[1].entry.metadata.location, "Bangladesh");
  REQUIRE_EQ(result.second[1].entry.metadata.rating, 4.8);
  REQUIRE_EQ(result.second[1].entry.feedback.index(), 1);
  REQUIRE_EQ(std::get<std::string>(result.second[1].entry.feedback),
             "Good app");
}
