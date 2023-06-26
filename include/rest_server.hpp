#pragma once

#include "coro.hpp"
#include "http_ops.hpp"
#include <boost/asio.hpp>
#include <boost/beast.hpp>
#include <string>
#include <unordered_map>

namespace rd {
namespace asio = boost::asio;
namespace http = boost::beast::http;
namespace beast = boost::beast;
using tcp = boost::asio::ip::tcp;

class rest_server {
public:
  using handler_t = std::function<asio::awaitable<void>(
      http::request<http::string_body> &, tcp::socket &)>;
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
      co_await std::invoke(it->second, req, socket);
    } else {
      co_await send_not_found_response(socket);
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
