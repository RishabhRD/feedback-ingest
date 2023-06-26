#include "application_state.hpp"
#include "tenant/read_tenant_file.hpp"
#include <doctest/doctest.h>

TEST_CASE("Check if all tenenants are being registered correctly") {
  std::vector<std::string> file_content{
      "1 enterpret@enterpret.com Enterpret",
      "   ",
      "2 rishabhdwivedi17@gmail.com Local",
      "",
  };
  rd::application_state_t state;
  auto mapping = rd::register_all_tenants(state, file_content);

  auto value_1 = state.tenant_registry.get(mapping[1]);
  REQUIRE_EQ(value_1.owner_email, "enterpret@enterpret.com");
  REQUIRE_EQ(value_1.organisation, "Enterpret");
  auto value_2 = state.tenant_registry.get(mapping[2]);
  REQUIRE_EQ(value_2.owner_email, "rishabhdwivedi17@gmail.com");
  REQUIRE_EQ(value_2.organisation, "Local");
}
