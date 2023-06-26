#include "data_source/discourse/load_discourse_file.hpp"
#include <doctest/doctest.h>

TEST_CASE("Creates correct data loader entries") {
  std::vector<std::string> file_content{
      "Discourse 111143 2 1 2",
      "  ",
      "Discourse 31963 2 1 2",
      "",
  };

  std::unordered_map<int, int> entry_map{{1, 12}, {2, 13}};

  auto const entries =
      rd::discourse::make_loader_entries(entry_map, file_content);

  REQUIRE_EQ(entries.size(), 2);
  REQUIRE_EQ(entries[0].subscribing_tenants.size(), 2);
  REQUIRE_EQ(entries[0].subscribing_tenants[0], 12);
  REQUIRE_EQ(entries[0].subscribing_tenants[1], 13);
  REQUIRE_EQ(entries[0].subscribing_tenants, std::vector<int>{12, 13});
  REQUIRE_EQ(entries[0].source_info_with_header.name, "Discourse");
  REQUIRE_EQ(entries[0].source_info_with_header.source_info.topic_id, 111143);
  REQUIRE_EQ(entries[1].subscribing_tenants.size(), 2);
  REQUIRE_EQ(entries[1].subscribing_tenants[0], 12);
  REQUIRE_EQ(entries[1].subscribing_tenants[1], 13);
  REQUIRE_EQ(entries[1].source_info_with_header.name, "Discourse");
  REQUIRE_EQ(entries[1].source_info_with_header.source_info.topic_id, 31963);
}
