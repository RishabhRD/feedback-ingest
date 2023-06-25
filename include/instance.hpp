#pragma once

#include <utility>

namespace rd {

template <typename key_t, typename value_t> struct instance_t {
  key_t instance_id;
  value_t value;
};

template <typename key_t, typename value_t>
auto to_instance(key_t key, value_t &value) -> rd::instance_t<key_t, value_t> {
  return {
      .instance_id = std::move(key),
      .value = std::move(value),
  };
}

} // namespace rd
