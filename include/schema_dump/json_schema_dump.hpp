#pragma once

#include "file_ops.hpp"
#include "schema.hpp"
#include "schema_serializers/json_serializer.hpp"
#include <chrono>
#include <fstream>

namespace rd {
inline auto
create_json_file_name(std::string base_path,
                      std::chrono::system_clock::time_point cur_time,
                      rd::schema_t const &schema) {
  return base_path + "/" + schema.tenant_id + "/" + schema.source_id + +"_" +
         std::to_string(cur_time.time_since_epoch().count()) + ".json";
}

inline auto save_schema_to_json(rd::schema_t const &schema) {
  std::string const base_path = ".";
  rd::json j = rd::serialize_to_json(schema);
  auto const file_name = create_json_file_name(
      base_path, std::chrono::system_clock::now(), schema);
  return rd::write_file(j, file_name);
}
} // namespace rd
