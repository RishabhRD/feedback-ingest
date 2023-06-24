#pragma once

#include "rest_server.hpp"
#include <boost/beast/http/string_body.hpp>
namespace rd {
namespace __detail {
inline rd::awaitable<void> reply_200_ok(http::request<http::string_body> &req,
                                        tcp::socket &socket) {
  http::response<http::string_body> res;
  res.version(req.version());
  res.result(http::status::ok);
  res.set(http::field::server, "Enterpret Rest Server");
  res.set(http::field::content_type, "text/plain");
  res.body() = "200 OK\r\n";
  res.prepare_payload();

  co_await http::async_write(socket, res, asio::use_awaitable);
}
} // namespace __detail

class read_only_rest_server {
  rd::rest_server server;

public:
  using awaitable_function_t =
      std::function<asio::awaitable<void>(http::request<http::string_body>)>;
  using normal_function_t =
      std::function<void(http::request<http::string_body>)>;
  using handler_t = handler_function_t<normal_function_t, awaitable_function_t>;
  read_only_rest_server(unsigned short port) : server(port) {}

  // Precondition: handler should not throw any exception
  void register_route(std::string const &path, handler_t f) {
    server.register_route(
        path, rest_server::awaitable_function_t{
                  [c_f = std::move(f)](
                      http::request<http::string_body> &req,
                      tcp::socket &socket) mutable -> rd::awaitable<void> {
                    co_await __detail::reply_200_ok(req, socket);
                    co_await invoke_handler_function(c_f, req);
                  }});
  }
  auto start_server() { return server.start_server(); }
};
} // namespace rd
