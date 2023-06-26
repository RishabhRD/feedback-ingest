#pragma once

#include "data_source/discourse/discourse_operation_state.hpp"
#include "data_source/filepp/filepp_operation_state.hpp"
#include "meta.hpp"
#include "timer/asio_timer.hpp"
#include <variant>

namespace rd {

using data_source_operation_state_t = std::variant<
    discourse::discourse_operation_state_t<rd::asio_timer::execute_every_t>,
    filepp::filepp_operation_state_t<rd::asio_timer::execute_every_t>>;

inline auto start(data_source_operation_state_t &op_state_)
    -> rd::awaitable<void> {
  return std::visit([](auto &op_state) { return op_state.start(); }, op_state_);
}

} // namespace rd
