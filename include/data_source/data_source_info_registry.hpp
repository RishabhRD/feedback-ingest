#pragma once

#include "data_source/data_source_info.hpp"
#include "registry/in_memory_registry.hpp"
namespace rd {
using data_source_info_registry_t =
    rd::in_memory_registry<rd::data_source_info_t>;
}
