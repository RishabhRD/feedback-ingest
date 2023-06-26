#pragma once

#include <boost/asio.hpp>
#include <boost/asio/async_result.hpp>

namespace rd {
template <typename T> using awaitable = boost::asio::awaitable<T>;
constexpr auto cur_ex = boost::asio::this_coro::executor;
constexpr auto use_awaitable = boost::asio::use_awaitable;

template <typename Duration, typename CompletionSignature>
boost::asio::awaitable<void> sleep(Duration duration,
                                   CompletionSignature &&sign) {
  boost::asio::system_timer timer(co_await boost::asio::this_coro::executor,
                                  duration);
  co_await timer.async_wait(sign);
}

template <typename T>
concept is_awaitable = requires(T) {
  typename T::value_type;
  std::same_as<T, rd::awaitable<typename T::value_type>>;
};
static_assert(rd::is_awaitable<rd::awaitable<void>>);
static_assert(rd::is_awaitable<rd::awaitable<int>>);

} // namespace rd
