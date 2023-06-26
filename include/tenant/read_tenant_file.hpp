#pragma once

#include "application_state.hpp"
#include "file_ops.hpp"
#include "tenant/tenant_info.hpp"
#include "tenant/tenant_registry.hpp"
#include "utils.hpp"
#include <sstream>
namespace rd {
namespace __details {
inline void register_entry(std::string const &entry,
                           std::unordered_map<int, int> &entry_map,
                           rd::tenant_registry_t &tenant_registry) {
  std::stringstream stream(entry);
  int file_index;
  rd::tenant_info_t tenant_info;
  stream >> file_index;
  stream >> tenant_info.owner_email >> tenant_info.organisation;
  auto actual_index = tenant_registry.register_value(tenant_info);
  entry_map[file_index] = actual_index;
}
} // namespace __details

std::unordered_map<int, int> inline register_all_tenants(
    rd::tenant_registry_t &tenant_registry,
    std::vector<std::string> const &entries) {
  std::unordered_map<int, int> mp;
  for (auto const &entry : without_blank_lines(entries)) {
    __details::register_entry(entry, mp, tenant_registry);
  }
  return mp;
}

std::unordered_map<int, int> inline load_tenant_file(
    std::string file_path, rd::tenant_registry_t &tenant_registry) {
  auto lines = rd::sync_read_file_lines(file_path);
  return register_all_tenants(tenant_registry, lines);
}

} // namespace rd
