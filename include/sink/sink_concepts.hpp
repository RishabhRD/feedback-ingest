#pragma once

#include "coro.hpp"
#include "schema.hpp"
#include <concepts>

namespace rd {
template <typename T>
concept is_sink = requires(T sink) {
  {
    sink.load(std::declval<std::vector<rd::schema_t>>())
  } -> std::same_as<rd::awaitable<void>>;
};
}
