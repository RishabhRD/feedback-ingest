#pragma once

#include "coro.hpp"
#include <boost/asio.hpp>
#include <boost/asio/experimental/awaitable_operators.hpp>
#include <boost/asio/ssl.hpp>
#include <boost/beast.hpp>
#include <boost/url.hpp>
#include <chrono>

namespace rd {

inline auto
make_http_get_header(boost::urls::result<boost::urls::url_view> parsed_url) {
  namespace http = boost::beast::http;

  http::request<http::string_body> req;
  req.method(http::verb::get);
  req.target(parsed_url->encoded_target() == "" ? "/"
                                                : parsed_url->encoded_target());
  req.set(http::field::host, parsed_url->encoded_host());
  req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
  req.prepare_payload();

  return req;
}

namespace __detail {
namespace beast = boost::beast;
namespace http = beast::http;
namespace asio = boost::asio;
using tcp = boost::asio::ip::tcp;

inline rd::awaitable<std::string>
make_get_request_to_socket(auto &socket,
                           http::request<http::string_body> request) {
  using namespace boost::asio::experimental::awaitable_operators;
  auto ctx = co_await asio::this_coro::executor;
  co_await http::async_write(socket, request, asio::use_awaitable);

  http::response<http::dynamic_body> response;
  beast::flat_buffer buffer;
  auto timeoutDuration = std::chrono::seconds(5);
  asio::steady_timer timer(ctx, timeoutDuration);

  co_await (http::async_read(socket, buffer, response, asio::use_awaitable) ||
            timer.async_wait(asio::use_awaitable));

  if (timer.expires_from_now() <= std::chrono::seconds(0)) {
    throw std::runtime_error("Request timed out");
  }
  co_return boost::beast::buffers_to_string(response.body().data());
}
} // namespace __detail

inline rd::awaitable<std::string> http_get(std::string const &url) {
  namespace urls = boost::urls;
  using tcp = boost::asio::ip::tcp;
  namespace asio = boost::asio;
  auto ctx = co_await rd::cur_ex;

  auto parsed_url = urls::parse_uri(url);
  auto req = make_http_get_header(parsed_url);
  tcp::resolver resolver(ctx);
  auto const endpoints = co_await resolver.async_resolve(
      parsed_url->encoded_host(),
      parsed_url->has_port() ? parsed_url->port() : parsed_url->scheme(),
      rd::use_awaitable);

  if (parsed_url->scheme() == "http") {
    tcp::socket socket(ctx);
    co_await asio::async_connect(socket, endpoints, rd::use_awaitable);
    auto str = co_await __detail::make_get_request_to_socket(socket, req);
    co_return str;
  } else {
    asio::ssl::context ssl_ctx(asio::ssl::context::tlsv12_client);
    asio::ssl::stream<asio::basic_stream_socket<tcp>> socket(ctx, ssl_ctx);
    co_await asio::async_connect(socket.next_layer(), endpoints,
                                 asio::use_awaitable);
    socket.handshake(asio::ssl::stream_base::client);
    auto str = co_await __detail::make_get_request_to_socket(socket, req);
    co_return str;
  }
}
} // namespace rd
