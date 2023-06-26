#include "application_state.hpp"
#include "data_source/discourse/load_discourse_file.hpp"
#include "data_source/filepp/load_filepp_file.hpp"
#include "data_source/notifyu/load_notifyu_file.hpp"
#include "tenant/read_tenant_file.hpp"
#include <boost/asio/detached.hpp>
#include <boost/asio/io_context.hpp>

// auto fetch_http() -> rd::awaitable<void> {
//   try {
//     auto res = co_await rd::http_get( //"http://127.0.0.1");
//         "https://meta.discourse.org/t/111021/"
//         "posts.json?post_ids%5B%5D=874799");
//     // "https://meta.discourse.org/"
//     // "search.json?page=1&q=after%3A2021-01-01+before%3A2021-02-20");
//
//     std::cout << res << std::endl;
//   } catch (std::exception &e) {
//
//     std::cout << e.what() << std::endl;
//   }
// }
//
// using namespace boost::beast;

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
