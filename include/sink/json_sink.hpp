#pragma once

#include "constants.hpp"
#include "coro.hpp"
#include "file_ops.hpp"
#include "schema.hpp"
#include "schema_serializers/json_serializer.hpp"
#include <filesystem>

namespace rd {

inline auto
create_json_file_name(std::string base_path,
                      std::chrono::system_clock::time_point cur_time,
                      int source_id, int tenant_id) {
  return base_path + "/" + std::to_string(tenant_id) + "/" +
         std::to_string(source_id) + +"_" +
         std::to_string(cur_time.time_since_epoch().count()) + ".json";
}

inline auto create_dir_name(std::string base_path, int tenant_id) {
  return base_path + "/" + std::to_string(tenant_id) + "/";
}

struct json_sink_t {
  auto write(std::vector<rd::schema_t> const &schemas) -> rd::awaitable<void> {
    if (schemas.size() == 0)
      co_return;
    int const source_id = schemas.front().source_id;
    int const tenant_id = schemas.front().tenant_id;
    auto const dir_name = create_dir_name(rd::schema_base_path, tenant_id);
    std::filesystem::create_directories(dir_name);
    rd::json j = rd::serialize_to_json(schemas);
    auto const file_name = create_json_file_name(
        rd::schema_base_path, std::chrono::system_clock::now(), source_id,
        tenant_id);
    co_await rd::write_file(j, file_name);
  }
};
} // namespace rd
