#include "rest_server.hpp"
#include "rest_server_concepts.hpp"
#include <doctest/doctest.h>

TEST_CASE("is_rest_server concept test") {
  static_assert(rd::is_rest_server<rd::rest_server>);
}
