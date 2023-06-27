#pragma once

#include "coro.hpp"

namespace rd {
struct any_operation_state {
  template <typename T>
  any_operation_state(T &&op_state_)
      : op_state(std::make_unique<model<T>>(std::forward<T>(op_state_))) {}

  auto start() { return op_state->start(); }

private:
  struct concept_t {
    virtual ~concept_t() = default;
    virtual rd::awaitable<void> start() = 0;
  };

  template <typename T> struct model : concept_t {
    model(T &&t) : object(std::forward<T>(t)) {}
    rd::awaitable<void> start() { return object.start(); }

  private:
    T object;
  };

  std::unique_ptr<concept_t> op_state;
};
} // namespace rd
