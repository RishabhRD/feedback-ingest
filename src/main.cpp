#include "http_ops.hpp"
#include <boost/asio.hpp>
#include <iostream>

auto fetch_http() -> rd::awaitable<void> {
  try {
    auto res = co_await rd::http_get("http://127.0.0.1");
    // "https://meta.discourse.org/"
    // "search.json?page=1&q=after%3A2021-01-01+before%3A2021-02-20");

    std::cout << res << std::endl;
  } catch (std::exception &e) {

    std::cout << e.what() << std::endl;
  }
}

int main() {
  boost::asio::io_context ctx;
  boost::asio::co_spawn(ctx, fetch_http(), boost::asio::detached);
  ctx.run();
}
