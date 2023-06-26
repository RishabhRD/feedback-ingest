#pragma once

#include "data_source/data_source_info_header.hpp"
#include "data_source/data_source_loader.hpp"
#include "data_source/filepp/filepp_info.hpp"
#include "data_source/filepp/filepp_operation_state.hpp"
#include "data_source/types.hpp"

namespace rd {
namespace filepp {
using info_t = rd::data_source_info_header_t<rd::filepp::filepp_info_t>;

namespace __detail {
inline rd::data_load_entry_t<rd::filepp::filepp_info_t>
get_loader_entry(std::unordered_map<int, int> const &entry_map,
                 std::string const &entry) {
  std::stringstream stream(entry);
  rd::data_source_info_header_t<rd::filepp::filepp_info_t> info;
  stream >> info.name >> info.source_info.file_path;
  std::vector<int> tenants;
  int n;
  stream >> n;
  for (int i = 0; i < n; ++i) {
    int x;
    stream >> x;
    tenants.push_back(entry_map.at(x));
  }
  return {
      .source_info_with_header = std::move(info),
      .subscribing_tenants = std::move(tenants),
  };
}
} // namespace __detail

constexpr auto make_loader_entries =
    rd::data_loader_utils::with_lines(__detail::get_loader_entry);

inline auto make_operation_state(int source_id, int tenant_id,
                                 rd::filepp::filepp_info_t source_info) {
  return rd::filepp::filepp_operation_state_t{
      source_id, tenant_id, source_info, rd::asio_timer::execute_every_t{}};
}

inline auto load_from_file(std::string file_path,
                           std::unordered_map<int, int> const &entry_map,
                           rd::application_state_t &state) {
  rd::load_from_file(std::move(file_path), entry_map, state,
                     make_loader_entries, make_operation_state);
}
} // namespace filepp
} // namespace rd
