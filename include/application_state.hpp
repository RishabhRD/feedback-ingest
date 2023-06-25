#pragma once

#include "data_source/data_source_info_registry.hpp"
#include "data_source/data_source_operation_state_registry.hpp"
#include "tenant/tenant_registry.hpp"
namespace rd {
struct application_state_t {
  rd::tenant_registry_t tenant_registry;
  rd::data_source_info_registry_t data_source_info_registry;
  rd::data_source_operation_state_registry_t
      data_source_operation_state_registry;
};
} // namespace rd
