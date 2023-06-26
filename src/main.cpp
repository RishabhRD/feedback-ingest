#include "application_state.hpp"
#include "data_source/discourse/load_discourse_file.hpp"
#include "data_source/filepp/load_filepp_file.hpp"
#include "tenant/read_tenant_file.hpp"

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
  rd::application_state_t state;
  auto entries = rd::load_tenant_file(rd::tenant_loader_path, state);
  rd::discourse::load_from_file(rd::discourse_loader_path, entries, state);
  rd::filepp::load_from_file(rd::filepp_loader_path, entries, state);
  state.data_source_operation_state_registry.ctx.run();
}
