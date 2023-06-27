#include "application_state.hpp"
#include "data_source/data_source_info_registry.hpp"
#include "data_source/data_source_operation_state_registry.hpp"
#include "data_source/discourse/load_discourse_file.hpp"
#include "data_source/filepp/load_filepp_file.hpp"
#include "data_source/notifyu/load_notifyu_file.hpp"
#include "sink/json_sink.hpp"
#include "spdlog/spdlog.h"
#include "tenant/read_tenant_file.hpp"
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>

auto make_application_state(boost::asio::io_context &ctx) {
  rd::data_source_operation_state_registry_t operation_state_registry(ctx);
  rd::rest_server server(4000);
  rd::json_sink_t sink;
  rd::application_state_t state(std::move(operation_state_registry),
                                std::move(server), std::move(sink));
  return state;
}

int main() {
  boost::asio::io_context ctx;
  spdlog::info("Started feedback-ingest...");
  auto state = make_application_state(ctx);
  auto entries =
      rd::load_tenant_file(rd::tenant_loader_path, state.tenant_registry);
  rd::discourse::load_from_file(rd::discourse_loader_path, entries, state);
  rd::filepp::load_from_file(rd::filepp_loader_path, entries, state);
  rd::notifyu::load_from_file(rd::notifyu_loader_path, entries, state);
  spdlog::info("Loaded data source files");
  boost::asio::co_spawn(ctx, state.server.start_server(),
                        boost::asio::detached);
  ctx.run();
}
