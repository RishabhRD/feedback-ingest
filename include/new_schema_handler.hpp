#pragma once

#include "schema.hpp"
#include "schema_dump/json_schema_dump.hpp"
#include <boost/asio/awaitable.hpp>

auto inline on_new_schema_creation(rd::schema_t const &schema)
    -> boost::asio::awaitable<void> {
  rd::save_schema_to_json(schema);
  return {};
}
