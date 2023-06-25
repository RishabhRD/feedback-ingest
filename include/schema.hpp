#pragma once

#include "optional.hpp"
#include <string>
#include <variant>
#include <vector>

namespace rd {

struct user_1 {};
struct user_2 {};

using user_t = std::variant<user_1, user_2>;

struct conversation_entry_t {
  user_t user;
  std::string message;
};

using conversation = std::vector<conversation_entry_t>;
using review = std::string;

using feedback_t = std::variant<conversation, review>;

// NOTE: Can include more optional fields as per more data sources
struct metadata_t {
  tl::optional<std::string> location;
  tl::optional<std::string> app_version;
  tl::optional<int> rating;
  tl::optional<int> impressions;
};

struct data_source_entry_t {
  metadata_t metadata;
  feedback_t feedback;
};

struct schema_t {
  std::string source_id;
  std::string tenant_id;
  data_source_entry_t entry;
};
} // namespace rd
