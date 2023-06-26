#include "application_state.hpp"
#include "data_source/discourse/load_discourse_file.hpp"
#include "data_source/filepp/load_filepp_file.hpp"
#include "data_source/notifyu/load_notifyu_file.hpp"
#include "tenant/read_tenant_file.hpp"
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>

int main() {
  boost::asio::io_context ctx;
  rd::application_state_t state(4000, ctx);
  auto entries =
      rd::load_tenant_file(rd::tenant_loader_path, state.tenant_registry);
  rd::discourse::load_from_file(rd::discourse_loader_path, entries, state);
  rd::filepp::load_from_file(rd::filepp_loader_path, entries, state);
  rd::notifyu::load_from_file(rd::notifyu_loader_path, entries, state);
  boost::asio::co_spawn(ctx, state.server.start_server(),
                        boost::asio::detached);
  ctx.run();
}
