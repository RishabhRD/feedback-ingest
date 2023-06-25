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
                      int source_id, int tenant_id) {
  return base_path + "/" + std::to_string(tenant_id) + "/" +
         std::to_string(source_id) + +"_" +
         std::to_string(cur_time.time_since_epoch().count()) + ".json";
}

inline auto save_schema_list_to_json(std::vector<rd::schema_t> const &schemas)
    -> rd::awaitable<void> {
  if (schemas.size() == 0)
    co_return;
  int const source_id = schemas.front().source_id;
  int const tenant_id = schemas.front().tenant_id;
  std::string const base_path = ".";
  rd::json j = rd::serialize_to_json(schemas);
  auto const file_name = create_json_file_name(
      base_path, std::chrono::system_clock::now(), source_id, tenant_id);
  co_await rd::write_file(j, file_name);
  co_return;
}
} // namespace rd
