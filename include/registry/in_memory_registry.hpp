#include "instance.hpp"
#include <unordered_map>
#include <vector>
namespace rd {
template <typename ValueType> struct in_memory_registry {
  using key_t = int;
  using value_t = ValueType;

  auto register_value(value_t value) -> key_t {
    int key;
    if (unused.size()) {
      registry[unused.back()] = std::move(value);
      key = unused.back();
      unused.pop_back();
    } else {
      key = cur_key;
      registry[cur_key++] = std::move(value);
    }
    return key;
  }

  // Postcondition: If key exist return key otherwise throw error
  auto get(key_t key) -> value_t & { return registry.at(key); }

  auto has(key_t key) -> bool { return registry.contains(key); }

  auto erase(key_t key) -> bool {
    if (registry.find(key) == registry.end())
      return false;

    registry.erase(key);
    unused.push_back(key);
    return true;
  }

private:
  key_t cur_key = 0;
  std::vector<key_t> unused;
  std::unordered_map<key_t, value_t> registry;
};
} // namespace rd
