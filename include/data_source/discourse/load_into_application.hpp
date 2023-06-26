#pragma once

#include "application_state.hpp"
#include "data_source/data_source_info_header.hpp"
#include "data_source/data_source_operation_state.hpp"
#include "data_source/discourse/discourse_operation_state.hpp"
#include "tenant/tenant_registry.hpp"
#include "timer/asio_timer.hpp"
namespace rd {
namespace discourse {

using info_t = rd::data_source_info_header_t<rd::discourse::discourse_info_t>;
using tenant_id_t = tenant_registry_t::key_t;

struct data_load_entry_t {
  info_t source_info_with_header;
  std::vector<int> subscribing_tenants;
};

inline auto load_into_application(std::vector<data_load_entry_t> data_to_load,
                                  application_state_t &state) {
  for (auto const &[source_info_with_header, tenants] : data_to_load) {
    auto const source_id =
        state.data_source_info_registry.register_value(source_info_with_header);
    for (auto tenant_id : tenants) {
      auto op_state = rd::discourse::discourse_operation_state_t{
          source_id, tenant_id, source_info_with_header.source_info,
          rd::asio_timer::execute_every_t{}};

      state.data_source_operation_state_registry.register_value(
          std::move(op_state));
    }
  }
}
} // namespace discourse
} // namespace rd
