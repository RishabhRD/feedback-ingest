#pragma once

#include <boost/asio.hpp>
#include <boost/asio/async_result.hpp>

namespace rd {
template <typename T> using awaitable = boost::asio::awaitable<T>;
constexpr auto cur_ex = boost::asio::this_coro::executor;
constexpr auto use_awaitable = boost::asio::use_awaitable;

template <typename T> awaitable<T> just(T value) { co_return value; }
inline awaitable<void> just() { co_return; }

template <typename Token, typename ErrorCode> struct AwaitableCompletionToken {
  Token token;
  ErrorCode &ec;

  void operator()(ErrorCode error, std::size_t /*bytes_transferred*/) {
    ec = error;
    token(std::move(ec));
  }
};

template <typename CompletionToken, typename ErrorCode>
  requires std::assignable_from<ErrorCode &, const ErrorCode &>
auto assign_error_code(CompletionToken &&token, ErrorCode &ec) {
  using Token = std::decay_t<CompletionToken>;
  return AwaitableCompletionToken<Token, ErrorCode>{
      std::forward<CompletionToken>(token), ec};
}

template <typename CompletionToken, typename ErrorCode, typename... Args>
  requires(!std::assignable_from<ErrorCode &, const ErrorCode &>)
auto assign_error_code(CompletionToken &&token, ErrorCode &ec, Args &&...args) {
  return [token, &ec](const ErrorCode &error, auto &&...innerArgs) mutable {
    ec = error.value(); // Extract error value or perform necessary conversion
    return token(std::move(ec),
                 std::forward<decltype(innerArgs)>(innerArgs)...);
  }(std::forward<Args>(args)...);
}

template <typename Duration, typename CompletionSignature>
boost::asio::awaitable<void> sleep(Duration duration,
                                   CompletionSignature &&sign) {
  boost::asio::steady_timer timer(co_await boost::asio::this_coro::executor,
                                  duration);
  co_await timer.async_wait(sign);
}

template <typename ErrorCode, typename... Args>
auto awaitable_with_error_code(ErrorCode &ec, Args &&...args) {
  return assign_error_code(boost::asio::use_awaitable, ec,
                           std::forward<Args>(args)...);
}
} // namespace rd
