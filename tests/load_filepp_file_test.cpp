#include "data_source/filepp/load_filepp_file.hpp"
#include <doctest/doctest.h>

TEST_CASE("Creates correct data loader entries") {
  std::vector<std::string> file_content{
      "filepp input_files/filepp_conversation_1.json 2 1 2",
      "  ",
      "filepp input_files/filepp_conversation_2.json 1 1",
      "",
  };

  std::unordered_map<int, int> entry_map{{1, 12}, {2, 13}};

  auto const entries = rd::filepp::make_loader_entries(entry_map, file_content);

  REQUIRE_EQ(entries.size(), 2);
  REQUIRE_EQ(entries[0].subscribing_tenants.size(), 2);
  REQUIRE_EQ(entries[0].subscribing_tenants[0], 12);
  REQUIRE_EQ(entries[0].subscribing_tenants[1], 13);
  REQUIRE_EQ(entries[0].subscribing_tenants, std::vector<int>{12, 13});
  REQUIRE_EQ(entries[0].source_info_with_header.name, "filepp");
  REQUIRE_EQ(entries[0].source_info_with_header.source_info.file_path,
             "input_files/filepp_conversation_1.json");
  REQUIRE_EQ(entries[1].subscribing_tenants.size(), 1);
  REQUIRE_EQ(entries[1].subscribing_tenants[0], 12);
  REQUIRE_EQ(entries[1].source_info_with_header.name, "filepp");
  REQUIRE_EQ(entries[1].source_info_with_header.source_info.file_path,
             "input_files/filepp_conversation_2.json");
}
