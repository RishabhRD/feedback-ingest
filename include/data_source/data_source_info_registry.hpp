#pragma once

#include "registry/in_memory_registry.hpp"
#include <any>
namespace rd {
using data_source_info_registry_t = rd::in_memory_registry<std::any>;
}
