#pragma once

#include "coro.hpp"
#include "data_source/filepp/algorithms.hpp"
#include "data_source/filepp/filepp_info.hpp"
#include "data_source/types.hpp"
#include "file_ops.hpp"
#include "new_schema_handler.hpp"
#include "utils.hpp"
#include <chrono>
#include <functional>
#include <spdlog/spdlog.h>

namespace rd {
namespace filepp {

template <typename Timer> struct filepp_operation_state_t {
  filepp_operation_state_t(source_id_t source_id_, tenant_id_t tenant_id_,
                           filepp_info_t filepp_info_, Timer schedule_every_)
      : source_id(std::move(source_id_)), tenant_id(std::move(tenant_id_)),
        filepp_info(std::move(filepp_info_)),
        schedule_every(std::move(schedule_every_)) {}

  filepp_operation_state_t(filepp_operation_state_t const &) = delete;
  filepp_operation_state_t(filepp_operation_state_t &&) = default;

  auto start() -> rd::awaitable<void> {
    try {
      auto op = [&](auto &&...) { return extract_transform_and_load(); };
      co_await schedule_every(std::chrono::minutes(30), op,
                              std::to_string(tenant_id) + "_" +
                                  std::to_string(source_id));
    } catch (std::exception &e) {
      spdlog::error("filepp source crashed \nsource_id: {}\ntenant_id: {}\n{}",
                    source_id, tenant_id, e.what());
    }
  }

private:
  auto extract_transform_and_load() -> rd::awaitable<void> {
    spdlog::info("filepp: Processing data for source_id: {}, tenant_id: {}",
                 source_id, tenant_id);
    auto const lines = co_await rd::read_file(filepp_info.file_path);
    auto const schema = rd::filepp::to_schema(source_id, tenant_id, lines);
    co_await rd::on_new_schema_creation(schema);
  }

  rd::source_id_t source_id;
  rd::tenant_id_t tenant_id;
  rd::filepp::filepp_info_t filepp_info;
  Timer schedule_every;
};
} // namespace filepp
} // namespace rd
