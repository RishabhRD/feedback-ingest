#pragma once

#include "coro.hpp"
#include "data_source/notifyu/algorithms.hpp"
#include "data_source/notifyu/notifyu_info.hpp"
#include "data_source/types.hpp"
#include "http_ops.hpp"
#include "rest_server_concepts.hpp"
#include "sink/sink_concepts.hpp"
#include "utils.hpp"
#include <boost/asio/co_spawn.hpp>
#include <boost/beast/http.hpp>
#include <functional>
#include <iostream>
#include <spdlog/spdlog.h>

namespace rd {
namespace notifyu {
template <rd::is_rest_server RestServer, rd::is_sink Sink>
struct notifyu_operation_state_t {
  notifyu_operation_state_t(source_id_t source_id_, tenant_id_t tenant_id_,
                            notifyu_info_t notifyu_info_,
                            std::reference_wrapper<RestServer> server_,
                            std::reference_wrapper<Sink> sink_)
      : source_id(std::move(source_id_)), tenant_id(std::move(tenant_id_)),
        notifyu_info(std::move(notifyu_info_)), server(std::move(server_)),
        sink(std::move(sink_)) {}

  notifyu_operation_state_t(notifyu_operation_state_t const &) = delete;
  notifyu_operation_state_t(notifyu_operation_state_t &&) = default;

  auto start() -> rd::awaitable<void> {
    try {
      auto op = [this](auto const &req, auto &socket) {
        return extract_transform_and_load(std::move(req), socket);
      };
      server.get().register_route(notifyu_info.listening_route, op);
      co_return;
    } catch (std::exception &e) {
      spdlog::error("notifyu source crashed");
      spdlog::error("source_id: {}", source_id);
      spdlog::error("tenant_id: {}", tenant_id);
      spdlog::error("exception info: {}", e.what());
    }
  }

private:
  auto extract_transform_and_load(
      boost::beast::http::request<boost::beast::http::string_body> const &req,
      boost::asio::ip::tcp::socket &socket) -> rd::awaitable<void> {
    try {
      spdlog::info("NotifyU: Processing data for source_id: {}, tenant_id: {}",
                   source_id, tenant_id);
      std::string body_data(req.body().data(),
                            req.body().data() + req.body().size());
      auto const parsed =
          rd::notifyu::parse_content(source_id, tenant_id, body_data);
      if (!is_good_secret_token(parsed.first)) {
        co_await rd::send_forbidden_response(socket);
        co_return;
      }
      co_await send_ok_response(socket);
      co_await sink.get().write(parsed.second);
    } catch (std::exception &e) {
      spdlog::error("notifyu request handler failed");
      spdlog::error("source_id: {}", source_id);
      spdlog::error("tenant_id: {}", tenant_id);
      spdlog::error("exception info: {}", e.what());
      co_return;
    }
  }

  rd::source_id_t source_id;
  rd::tenant_id_t tenant_id;
  rd::notifyu::notifyu_info_t notifyu_info;
  std::reference_wrapper<RestServer> server;
  std::reference_wrapper<Sink> sink;
};
} // namespace notifyu
} // namespace rd
