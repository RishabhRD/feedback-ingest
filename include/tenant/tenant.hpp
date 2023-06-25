#pragma once

#include "instance.hpp"
#include "tenant/tenant_info.hpp"
#include "tenant/tenant_registry.hpp"
namespace rd {
using tenant_t =
    rd::instance_t<rd::tenant_registry_t::key_t, rd::tenant_info_t>;
} // namespace rd
