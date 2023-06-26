#pragma once

#include "data_source/data_source_operation_state.hpp"
#include "registry/in_memory_registry.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/io_context.hpp>
namespace rd {

class data_source_operation_state_registry_t {
  rd::in_memory_registry<rd::data_source_operation_state_t> registry;

public:
  boost::asio::io_context ctx;
  using key_t = decltype(registry)::key_t;
  using value_t = decltype(registry)::value_t;

  // TODO: Make tasks cancellable from outside, and automatically remove when
  // done
  auto register_value(value_t value) -> key_t {
    auto res = registry.register_value(std::move(value));
    boost::asio::co_spawn(ctx, rd::start(registry.get(res)),
                          boost::asio::detached);
    return res;
  }
  // Question: Should these method even exist for operation states
  //
  // auto get(key_t key) -> value_t & { return registry.get(key); }
  //
  // auto has(key_t key) -> bool { return registry.has(key); }

  // TODO: Initiate cancellation of task
  auto remove(key_t key) -> bool { return registry.remove(key); }
};

} // namespace rd
