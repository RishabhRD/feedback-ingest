// #include "fake_timer.hpp"
// #include "timer/asio_timer.hpp"
// #include <boost/asio.hpp>
// #include <chrono>
// #include <doctest/doctest.h>
// #include <iostream>
//
// using namespace boost::asio;
// using namespace std::chrono;
//
// TEST_CASE("asio timer executes timely") {
//   std::vector<rd::fake_clock::time_point> timepoints;
//
//   io_context ctx;
//   co_spawn(ctx,
//            rd::asio_timer::execute_every(seconds(5),
//                                          [](auto old) {
//                                            std::cout
//                                                <<
//                                                old.time_since_epoch().count()
//                                                << std::endl;
//                                          }),
//            detached);
//
//   ctx.run();
// }
