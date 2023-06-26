#include "utils.hpp"
#include <doctest/doctest.h>
#include <range/v3/range/conversion.hpp>
#include <string>
#include <vector>

TEST_CASE("without blank lines") {
  std::vector<std::string> file_content{
      "1 enterpret@enterpret.com Enterpret",
      "   ",
      "2 rishabhdwivedi17@gmail.com Local)",
      "",
  };

  auto lines = rd::without_blank_lines(file_content) | ranges::to_vector;
  std::vector<std::string> expected_lines{
      "1 enterpret@enterpret.com Enterpret",
      "2 rishabhdwivedi17@gmail.com Local)",
  };

  REQUIRE_EQ(lines.size(), 2);
  REQUIRE_EQ(lines[0], expected_lines[0]);
  REQUIRE_EQ(lines[1], expected_lines[1]);
}
