#pragma once

#include "coro.hpp"
#include "overload.hpp"
#include <boost/asio.hpp>
#include <boost/asio/awaitable.hpp>
#include <boost/asio/co_spawn.hpp>
#include <boost/asio/detached.hpp>
#include <boost/beast.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/version.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <variant>

namespace rd {
namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;

template <typename normal_function_t, typename awaitable_function_t>
using handler_function_t =
    std::variant<normal_function_t, awaitable_function_t>;

template <typename normal_function_t, typename awaitable_function_t,
          typename... Args>
rd::awaitable<void> inline invoke_handler_function(
    handler_function_t<normal_function_t, awaitable_function_t> &f,
    Args &...args) {
  return std::visit(
      rd::overloaded{
          [&](normal_function_t &handler) -> rd::awaitable<void> {
            handler(args...);
            co_return;
          },
          [&](awaitable_function_t &handler) -> rd::awaitable<void> {
            co_await handler(args...);
          },
      },
      f);
}

class rest_server {
public:
  using awaitable_function_t = std::function<asio::awaitable<void>(
      http::request<http::string_body> &, tcp::socket &)>;
  using normal_function_t =
      std::function<void(http::request<http::string_body> &, tcp::socket &)>;
  using handler_t = handler_function_t<normal_function_t, awaitable_function_t>;
  rest_server(unsigned short port) : port_(port) {}

  // Precondition: handler should not throw any exception
  void register_route(const std::string &path, handler_t handler) {
    route_handlers_[path] = std::move(handler);
  }

  asio::awaitable<void> start_server() {
    auto io_context = co_await asio::this_coro::executor;
    tcp::acceptor acceptor(io_context, {tcp::v4(), port_});

    while (true) {
      tcp::socket socket(io_context);
      co_await acceptor.async_accept(socket, asio::use_awaitable);
      asio::co_spawn(io_context, session(std::move(socket)), asio::detached);
    }
  }

private:
  asio::awaitable<void> handle_request(http::request<http::string_body> &req,
                                       tcp::socket &socket) {
    auto it = route_handlers_.find(req.target());
    if (it != route_handlers_.end()) {
      co_await invoke_handler_function(it->second, req, socket);
    } else {
      http::response<http::string_body> res;
      res.version(req.version());
      res.result(http::status::not_found);
      res.set(http::field::server, "Enterpret Rest Server");
      res.set(http::field::content_type, "text/plain");
      res.body() = "404 Not Found\r\n";
      res.prepare_payload();

      co_await http::async_write(socket, res, asio::use_awaitable);
    }
  }

  asio::awaitable<void> session(tcp::socket socket) {
    beast::flat_buffer buffer;
    http::request<http::string_body> req;
    co_await http::async_read(socket, buffer, req, asio::use_awaitable);
    co_await handle_request(req, socket);
    boost::system::error_code ec;
    socket.shutdown(tcp::socket::shutdown_send, ec);
  }

  unsigned short port_;
  std::unordered_map<std::string, handler_t> route_handlers_;
};

} // namespace rd
