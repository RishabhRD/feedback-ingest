#pragma once

#include "coro.hpp"
#include <fstream>
#include <sstream>
#include <string>
#include <string_view>

namespace rd {
inline auto read_file(std::string file_path) -> rd::awaitable<std::string> {
  std::ifstream in(file_path);
  std::stringstream buffer;
  buffer << in.rdbuf();

  return rd::just(buffer.str());
}

inline auto read_file_lines(std::string file_path)
    -> rd::awaitable<std::vector<std::string>> {
  std::vector<std::string> res;
  std::ifstream in(file_path);
  std::string line;
  while (std::getline(in, line)) {
    res.push_back(line);
  }

  return rd::just(std::move(res));
}

template <typename Content>
inline auto write_file(Content content, std::string file_path)
    -> rd::awaitable<void> {
  std::ofstream of(file_path);
  of << content;

  return rd::just();
}

} // namespace rd
