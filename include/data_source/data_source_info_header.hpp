#pragma once

#include <string>
namespace rd {
template <typename data_source_t> struct data_source_info_header_t {
  std::string name;
  data_source_t source_info;
};
} // namespace rd
