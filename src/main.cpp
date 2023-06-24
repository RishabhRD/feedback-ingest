#include "http_ops.hpp"
#include "read_only_rest_server.hpp"
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

using namespace boost::beast;

int main() {
  boost::asio::io_context ctx;
  rd::read_only_rest_server server(4000);
  server.register_route("/", [](http::request<http::string_body> req) {
    std::cout << "handling /" << std::endl;
    std::cout << req.body() << std::endl;
  });
  server.register_route("/home", [](http::request<http::string_body> req) {
    std::cout << "handling /home" << std::endl;
    std::cout << req.body() << std::endl;
  });
  boost::asio::co_spawn(ctx, server.start_server(), boost::asio::detached);
  ctx.run();
}
