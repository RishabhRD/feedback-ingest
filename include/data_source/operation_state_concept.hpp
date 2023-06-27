#pragma once

#include "coro.hpp"
#include <concepts>
namespace rd {
template <typename T>
concept is_operation_state = requires(T t) {
  !std::copy_constructible<T>;
  std::is_move_constructible_v<T>;
  { t.start() } -> std::same_as<rd::awaitable<void>>;
};
}
