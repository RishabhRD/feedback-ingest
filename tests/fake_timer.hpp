#pragma once

#include <chrono>
#include <cstdint>

namespace rd {
class fake_clock {
public:
  using rep = long;
  using period = std::ratio<1l>;
  using duration = std::chrono::duration<rep, period>;
  using time_point = std::chrono::time_point<fake_clock>;

  static void advance(duration d) noexcept { now_us_ += d; }

  static void reset() noexcept { now_us_ = time_point(); }

  static time_point now() noexcept { return now_us_; }

  fake_clock() = delete;

private:
  static time_point now_us_;
};
} // namespace rd

inline rd::fake_clock::time_point rd::fake_clock::now_us_;
