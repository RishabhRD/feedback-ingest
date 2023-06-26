#pragma once

#include "data_source/data_source_info_registry.hpp"
#include "data_source/data_source_operation_state_registry.hpp"
#include "rest_server.hpp"
#include "tenant/tenant_registry.hpp"
#include <boost/asio/io_context.hpp>
namespace rd {
struct application_state_t {
  rd::tenant_registry_t tenant_registry;
  rd::data_source_info_registry_t data_source_info_registry;
  rd::data_source_operation_state_registry_t
      data_source_operation_state_registry;
  rd::rest_server server;

  application_state_t(unsigned short port,
                      std::reference_wrapper<boost::asio::io_context> ctx)
      : data_source_operation_state_registry(std::move(ctx)), server(port) {}
};
} // namespace rd
