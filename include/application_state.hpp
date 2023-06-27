#pragma once

#include "data_source/data_source_info_registry.hpp"
#include "data_source/data_source_operation_state_registry.hpp"
#include "rest_server.hpp"
#include "sink/json_sink.hpp"
#include "tenant/tenant_registry.hpp"
#include <boost/asio/io_context.hpp>
namespace rd {
struct application_state_t {
  rd::tenant_registry_t tenant_registry;
  rd::data_source_info_registry_t data_source_info_registry;
  rd::data_source_operation_state_registry_t
      data_source_operation_state_registry;
  rd::rest_server server;
  rd::json_sink_t sink;

  application_state_t(rd::data_source_operation_state_registry_t registry_,
                      rd::rest_server server_, rd::json_sink_t sink_)
      : data_source_operation_state_registry(std::move(registry_)),
        server(std::move(server_)), sink(std::move(sink_)) {}
};
} // namespace rd
