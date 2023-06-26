#pragma once

#include "coro.hpp"
#include "optional.hpp"
#include "timer_concept.hpp"
#include <boost/asio.hpp>
#include <chrono>

namespace rd {

namespace asio_timer {
using namespace boost::asio;
using namespace std::chrono;

struct execute_every_t {
  using clock_t = std::chrono::system_clock;
  using timepoint_t = std::chrono::time_point<clock_t>;

  auto operator()(rd::is_duration auto duration, auto &callback) const {
    return execute(clock_t::now(), duration, callback, "");
  }

  auto operator()(rd::is_duration auto duration, auto &callback,
                  std::string dedup_id) const {
    return execute(clock_t::now(), duration, callback, dedup_id);
  }

  auto operator()(timepoint_t start_time, rd::is_duration auto duration,
                  auto &callback) const {
    return execute(start_time, duration, callback, "");
  }

  auto operator()(timepoint_t start_time, rd::is_duration auto duration,
                  auto &callback, std::string dedup_id) const {
    return execute(start_time, duration, callback, dedup_id);
  }

private:
  template <typename Callback>
  auto execute(timepoint_t start_time, rd::is_duration auto duration,
               Callback &callback, std::string) const -> awaitable<void> {
    system_timer timer(co_await this_coro::executor);
    co_await call_callback(callback, start_time);
    for (;;) {
      start_time = clock_t::now();
      timer.expires_after(duration);
      co_await timer.async_wait(use_awaitable);
      co_await call_callback(callback, start_time);
    }
  }

  template <typename Callback>
  auto call_callback(Callback &callback, timepoint_t time) const
      -> awaitable<void> {
    if constexpr (rd::is_awaitable<
                      std::invoke_result_t<Callback, timepoint_t>>) {
      co_await callback(time);
    } else {
      callback(time);
    }
  }
};

constexpr auto execute_every = execute_every_t{};

} // namespace asio_timer
} // namespace rd
