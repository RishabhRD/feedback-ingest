#include <doctest/doctest.h>
#include <registry/in_memory_registry.hpp>

TEST_CASE("Can get registered value with returned key") {
  rd::in_memory_registry<int> registry;

  auto key_1 = registry.register_value(1);
  auto key_2 = registry.register_value(2);

  REQUIRE_EQ(registry.get(key_1), 1);
  REQUIRE_EQ(registry.get(key_2), 2);
}

TEST_CASE("Should not contain value after erasing it") {
  rd::in_memory_registry<int> registry;

  auto key_1 = registry.register_value(1);
  registry.register_value(2);

  registry.erase(key_1);
  REQUIRE_EQ(registry.has(key_1), false);
}

TEST_CASE("Should contain value after adding it") {
  rd::in_memory_registry<int> registry;
  auto key_1 = registry.register_value(1);

  REQUIRE_EQ(registry.has(key_1), true);
}

TEST_CASE("Should throw exception when try to access non-existing key") {
  rd::in_memory_registry<int> registry;
  REQUIRE_THROWS(registry.get(1));
}

TEST_CASE("Should throw exception when try erased key (not added)") {
  rd::in_memory_registry<int> registry;
  auto key_1 = registry.register_value(1);
  registry.erase(key_1);
  REQUIRE_THROWS(registry.get(key_1));
}
