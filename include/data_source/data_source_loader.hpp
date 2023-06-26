#pragma once

#include "application_state.hpp"
#include "data_source/data_source_info_header.hpp"
#include "utils.hpp"
#include <vector>
namespace rd {
template <typename SourceInfo> struct data_load_entry_t {
  rd::data_source_info_header_t<SourceInfo> source_info_with_header;
  std::vector<int> subscribing_tenants;
};

template <typename SourceInfo>
inline auto
load_into_application(std::vector<data_load_entry_t<SourceInfo>> data_to_load,
                      application_state_t &state, auto &&make_operation_state) {
  for (auto const &[source_info_with_header, tenants] : data_to_load) {
    auto const source_id =
        state.data_source_info_registry.register_value(source_info_with_header);
    for (auto tenant_id : tenants) {
      state.data_source_operation_state_registry.register_value(
          make_operation_state(source_id, tenant_id,
                               source_info_with_header.source_info, state));
    }
  }
}

inline auto load_from_file(std::string file_path,
                           std::unordered_map<int, int> const &entry_map,
                           rd::application_state_t &state,
                           auto &&make_loader_entries,
                           auto &&make_operation_state) {
  auto const lines = rd::sync_read_file_lines(file_path);
  load_into_application(make_loader_entries(entry_map, lines), state,
                        make_operation_state);
}

namespace data_loader_utils {
constexpr auto with_lines = [](auto get_loader_entry_) {
  return [get_loader_entry = std::move(get_loader_entry_)](
             std::unordered_map<int, int> const &entry_map,
             std::vector<std::string> const &entries) {
    namespace rv = ranges::views;
    return                                                            //
        rd::without_blank_lines(entries)                              //
        | rv::transform(std::bind_front(get_loader_entry, entry_map)) //
        | ranges::to_vector;
  };
};
}
} // namespace rd
