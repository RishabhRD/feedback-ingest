#pragma once

#include "data_source/data_source_operation_state.hpp"
#include "registry/in_memory_registry.hpp"
#include <boost/asio/asio.hpp>

namespace rd {

class data_source_operation_state_registry_t {
  rd::in_memory_registry<rd::data_source_operation_state_t> registry;
  std::reference_wrapper<boost::asio::io_context> ctx;

public:
  data_source_operation_state_registry_t(
      std::reference_wrapper<boost::asio::io_context> ctx_)
      : ctx(std::move(ctx_)) {}

  using key_t = decltype(registry)::key_t;
  using value_t = decltype(registry)::value_t;

  // TODO: Make tasks cancellable from outside, and automatically remove when
  // done
  // Precondition: start task should not throw any exception
  auto register_value(value_t value) -> key_t {
    auto res = registry.register_value(std::move(value));
    boost::asio::co_spawn(ctx.get(), rd::start(registry.get(res)),
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
