#pragma once

#include "optional.hpp"
#include "timer_concept.hpp"
#include <boost/asio.hpp>
#include <chrono>
#include <iostream>

namespace rd {

namespace asio_timer {
using namespace boost::asio;
using namespace std::chrono;

struct execute_every_t {
  auto operator()(rd::is_duration auto duration, auto callback) const {
    return execute(duration, callback, "");
  }

  auto operator()(rd::is_duration auto duration, auto callback,
                  std::string dedup_id) const {
    return execute(duration, callback, dedup_id);
  }

private:
  auto execute(rd::is_duration auto duration, auto callback,
               std::string __dedup_id) const -> awaitable<void> {
    steady_timer timer(co_await this_coro::executor);
    for (;;) {
      auto const prev_time = steady_clock::now();
      timer.expires_after(duration);
      co_await timer.async_wait(use_awaitable);
      callback(prev_time);
    }
  }
};

constexpr auto execute_every = execute_every_t{};

} // namespace asio_timer
} // namespace rd
