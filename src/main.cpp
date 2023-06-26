#include "application_state.hpp"
#include "data_source/data_source_info_header.hpp"
#include "data_source/discourse/discourse_info.hpp"
#include "data_source/discourse/load_into_application.hpp"
#include "tenant/tenant_info.hpp"
#include <iostream>

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
  rd::tenant_info_t t1{
      .owner_email = "enterpret@enterpret.com",
      .organisation = "enterpret",
  };
  rd::tenant_info_t t2{
      .owner_email = "rishabhdwivedi17@gmail.com",
      .organisation = "local",
  };

  auto t1_id = state.tenant_registry.register_value(t1);
  auto t2_id = state.tenant_registry.register_value(t2);

  rd::data_source_info_header_t<rd::discourse::discourse_info_t> s1{
      .name = "Discourse",
      .source_info =
          rd::discourse::discourse_info_t{
              .topic_id = 111143,
          },
  };

  rd::data_source_info_header_t<rd::discourse::discourse_info_t> s2{
      .name = "Discourse",
      .source_info =
          rd::discourse::discourse_info_t{
              .topic_id = 31963,
          },
  };
  rd::discourse::load_into_application(t1_id, s1, state);
  rd::discourse::load_into_application(t1_id, s2, state);
  rd::discourse::load_into_application(t2_id, s1, state);
  rd::discourse::load_into_application(t2_id, s2, state);
  state.data_source_operation_state_registry.ctx.run();
}
