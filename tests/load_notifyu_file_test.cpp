#include "data_source/notifyu/load_notifyu_file.hpp"
#include <doctest/doctest.h>

TEST_CASE("Creates correct data loader entries") {
  std::vector<std::string> file_content{
      "notifyu /notifyu_path_1 2",
      "  ",
      "notifyu /notifyu_path_2 1",
      "",
  };

  std::unordered_map<int, int> entry_map{{1, 12}, {2, 13}};

  auto const entries =
      rd::notifyu::make_loader_entries(entry_map, file_content);

  REQUIRE_EQ(entries.size(), 2);
  REQUIRE_EQ(entries[0].subscribing_tenants.size(), 1);
  REQUIRE_EQ(entries[0].subscribing_tenants[0], 13);
  REQUIRE_EQ(entries[0].source_info_with_header.name, "notifyu");
  REQUIRE_EQ(entries[0].source_info_with_header.source_info.listening_route,
             "/notifyu_path_1");
  REQUIRE_EQ(entries[1].subscribing_tenants.size(), 1);
  REQUIRE_EQ(entries[1].subscribing_tenants[0], 12);
  REQUIRE_EQ(entries[1].source_info_with_header.name, "notifyu");
  REQUIRE_EQ(entries[1].source_info_with_header.source_info.listening_route,
             "/notifyu_path_2");
}
