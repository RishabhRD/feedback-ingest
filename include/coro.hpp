#pragma once

#include <boost/asio.hpp>
#include <boost/asio/async_result.hpp>

namespace rd {
template <typename T> using awaitable = boost::asio::awaitable<T>;
constexpr auto cur_ex = boost::asio::this_coro::executor;
constexpr auto use_awaitable = boost::asio::use_awaitable;

template <typename T> awaitable<T> just(T value) { co_return value; }
inline awaitable<void> just() { co_return; }
} // namespace rd
