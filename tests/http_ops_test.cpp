#include "http_ops.hpp"
#include <boost/url/parse.hpp>
#include <doctest/doctest.h>

TEST_CASE("Makes correct HTTP Get header") {
  SUBCASE("https://www.google.com") {
    auto header = rd::make_http_get_header(
        boost::urls::parse_uri("https://www.google.com"));
    REQUIRE_EQ(header.version(), 11);
    REQUIRE_EQ(header.method_string(), "GET");
    REQUIRE_EQ(header.target(), "/");
  }
  SUBCASE("https://www.google.com/") {
    auto header = rd::make_http_get_header(
        boost::urls::parse_uri("https://www.google.com/"));
    REQUIRE_EQ(header.version(), 11);
    REQUIRE_EQ(header.method_string(), "GET");
    REQUIRE_EQ(header.target(), "/");
  }
  SUBCASE("https://www.google.com/tmp") {
    auto header = rd::make_http_get_header(
        boost::urls::parse_uri("https://www.google.com/tmp"));
    REQUIRE_EQ(header.version(), 11);
    REQUIRE_EQ(header.method_string(), "GET");
    REQUIRE_EQ(header.target(), "/tmp");
  }
  SUBCASE("https://www.google.com/tmp/something") {
    auto header = rd::make_http_get_header(
        boost::urls::parse_uri("https://www.google.com/tmp/something"));
    REQUIRE_EQ(header.version(), 11);
    REQUIRE_EQ(header.method_string(), "GET");
    REQUIRE_EQ(header.target(), "/tmp/something");
  }
  // SUBCASE("https://www.google.com/tmp/something:4000") {
  //   auto header = rd::make_http_get_header(
  //       boost::urls::parse_uri("https://www.google.com/tmp/something:4000"));
  //   REQUIRE_EQ(header.version(), 11);
  //   REQUIRE_EQ(header.method_string(), "GET");
  //   REQUIRE_EQ(header.target(), "/tmp/something");
  // }
}
