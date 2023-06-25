#pragma once

#include "optional.hpp"
#include "timer_concept.hpp"
#include <boost/asio.hpp>
#include <chrono>

namespace rd {

namespace asio_timer {
using namespace boost::asio;
using namespace std::chrono;

struct execute_every_t {
  using clock_t = std::chrono::steady_clock;
  using timepoint_t = std::chrono::time_point<clock_t>;

  auto operator()(rd::is_duration auto duration, auto callback) const {
    return execute(clock_t::now(), duration, callback, "");
  }

  auto operator()(rd::is_duration auto duration, auto callback,
                  std::string dedup_id) const {
    return execute(clock_t::now(), duration, callback, dedup_id);
  }

  auto operator()(timepoint_t start_time, rd::is_duration auto duration,
                  auto callback) const {
    return execute(start_time, duration, callback, "");
  }

  auto operator()(timepoint_t start_time, rd::is_duration auto duration,
                  auto callback, std::string dedup_id) const {
    return execute(start_time, duration, callback, dedup_id);
  }

private:
  auto execute(timepoint_t start_time, rd::is_duration auto duration,
               auto callback, std::string __dedup_id) const -> awaitable<void> {
    steady_timer timer(co_await this_coro::executor);
    for (;;) {
      timer.expires_after(duration);
      co_await timer.async_wait(use_awaitable);
      callback(start_time);
      start_time = clock_t::now();
    }
  }
};

constexpr auto execute_every = execute_every_t{};

} // namespace asio_timer
} // namespace rd
