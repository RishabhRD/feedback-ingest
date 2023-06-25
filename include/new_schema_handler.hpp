#pragma once

#include "schema.hpp"
#include "schema_dump/json_schema_dump.hpp"

auto inline on_new_schema_creation(std::vector<rd::schema_t> const &schema) {
  return rd::save_schema_list_to_json(schema);
}
