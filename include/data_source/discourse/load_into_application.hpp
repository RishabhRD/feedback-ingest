#pragma once

#include "application_state.hpp"
#include "data_source/data_source_info_header.hpp"
#include "data_source/data_source_operation_state.hpp"
#include "timer/asio_timer.hpp"
namespace rd {
namespace discourse {

inline auto load_into_application(
    int tenant_id,
    rd::data_source_info_header_t<discourse_info_t> discourse_info,
    rd::application_state_t &state) {
  auto const source_id =
      state.data_source_info_registry.register_value(discourse_info);
  rd::discourse::discourse_operation_state_t op_state(
      source_id, tenant_id, discourse_info.source_info,
      rd::asio_timer::execute_every_t{});
  state.data_source_operation_state_registry.register_value(
      std::move(op_state));
}
} // namespace discourse
} // namespace rd
