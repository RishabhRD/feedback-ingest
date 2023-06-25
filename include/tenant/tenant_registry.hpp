#pragma once

#include "registry/in_memory_registry.hpp"
#include "tenant/tenant_info.hpp"
namespace rd {
using tenant_registry_t = in_memory_registry<rd::tenant_info_t>;
}
