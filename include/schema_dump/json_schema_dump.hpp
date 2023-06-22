#pragma once

#include "schema.hpp"
#include "schema_serializers/json_serializer.hpp"
#include <fstream>

namespace rd {
inline auto create_json_file_name(std::string base_path,
                                  rd::schema_t const &schema) {
  return base_path + "/" + schema.metadata.source + "_" + schema.metadata.id +
         ".json";
}

inline auto save_schema_to_json(rd::schema_t const &schema) {
  std::string const base_path = ".";
  rd::json j = rd::serialize_to_json(schema);
  auto const file_name = create_json_file_name(base_path, schema);
  std::ofstream outputfile(file_name);
  outputfile << j;
}
} // namespace rd
