#pragma once

#include <boost/hof/lift.hpp>
#include <cctype>
#include <range/v3/algorithm/all_of.hpp>
#include <range/v3/view/filter.hpp>
namespace rd {
constexpr auto without_blank_lines =
    ranges::views::filter([](std::string_view str) {
      return !ranges::all_of(str, BOOST_HOF_LIFT(std::isspace));
    });

constexpr auto ignore_arguments = [](auto &&f) {
  return [&f](auto &&...) { return f(); };
};
} // namespace rd
