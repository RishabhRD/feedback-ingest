#include "data_source/any_data_source_info.hpp"
// #include "data_source/discourse/data_source.hpp"
// #include "http_ops.hpp"
// #include "read_only_rest_server.hpp"
// #include <boost/asio.hpp>
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

  // boost::asio::io_context ctx;
  // using namespace std::chrono_literals;
  // auto source_id = 123;
  // auto tenant_id = 321;
  // auto begin_date = std::chrono::sys_days{2021y / 1 / 1};
  // auto end_date = std::chrono::sys_days{2021y / 1 / 30};
  // int topic_id = 111021;
  // auto post = rd::discourse::fetch_posts_between(source_id, tenant_id,
  // topic_id,
  //                                                begin_date, end_date);
  // boost::asio::co_spawn(ctx, std::move(post), boost::asio::detached);
  // ctx.run();
}
