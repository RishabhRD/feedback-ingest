#pragma once

#include "data_source/discourse/discourse_operation_state.hpp"
#include "meta.hpp"
#include "timer/asio_timer.hpp"

namespace rd {

using data_source_operation_state_t = std::variant<
    discourse::discourse_operation_state_t<rd::asio_timer::execute_every_t>>;

} // namespace rd
