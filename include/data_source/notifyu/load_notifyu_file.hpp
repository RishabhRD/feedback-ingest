#pragma once

#include "data_source/data_source_info_header.hpp"
#include "data_source/data_source_loader.hpp"
#include "data_source/notifyu/notifyu_info.hpp"
#include "data_source/notifyu/notifyu_operation_state.hpp"
#include "data_source/types.hpp"
#include <functional>

namespace rd {
namespace notifyu {
using info_t = rd::data_source_info_header_t<rd::notifyu::notifyu_info_t>;

namespace __detail {
inline rd::data_load_entry_t<rd::notifyu::notifyu_info_t>
get_loader_entry(std::unordered_map<int, int> const &entry_map,
                 std::string const &entry) {
  std::stringstream stream(entry);
  rd::data_source_info_header_t<rd::notifyu::notifyu_info_t> info;
  stream >> info.name >> info.source_info.listening_route;
  int tenant;
  stream >> tenant;
  return {
      .source_info_with_header = std::move(info),
      .subscribing_tenants = std::vector({entry_map.at(tenant)}),
  };
}
} // namespace __detail

constexpr auto make_loader_entries =
    rd::data_loader_utils::with_lines(__detail::get_loader_entry);

inline auto make_operation_state(int source_id, int tenant_id,
                                 rd::notifyu::notifyu_info_t source_info,
                                 rd::application_state_t &state) {
  return rd::notifyu::notifyu_operation_state_t{
      source_id, tenant_id, source_info, std::ref(state.server)};
}

inline auto load_from_file(std::string file_path,
                           std::unordered_map<int, int> const &entry_map,
                           rd::application_state_t &state) {
  rd::load_from_file(std::move(file_path), entry_map, state,
                     make_loader_entries, make_operation_state);
}
} // namespace notifyu
} // namespace rd
