#pragma once

#include "coro.hpp"
#include <concepts>
#include <string>

namespace rd {
template <typename T>
concept is_rest_server = requires(T server) {
  typename T::handler_t;
  {
    server.register_route(std::declval<std::string>(),
                          std::declval<typename T::handler_t>())
  } -> std::same_as<void>;

  { server.start_server() } -> std::same_as<rd::awaitable<void>>;
};
}
