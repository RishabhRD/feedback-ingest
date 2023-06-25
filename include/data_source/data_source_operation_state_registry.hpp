#pragma once

#include "data_source/data_source_operation_state.hpp"
#include "registry/in_memory_registry.hpp"
namespace rd {

// TODO: Need to handle graceful exit of task, by having cancellation
// Cancellation is in built with awaitables, need to put a decorator
// pattern around in_memory_registry to do the same
using data_source_operation_state_registry_t =
    rd::in_memory_registry<rd::data_source_operation_state_t>;
} // namespace rd
