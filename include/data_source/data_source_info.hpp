#pragma once

#include "data_source/discourse/discourse_info.hpp"
#include "data_source_info_header.hpp"
#include "meta.hpp"
#include "schema.hpp"
#include <variant>
namespace rd {
namespace __details {

template <typename... T>
using with_data_source_info_t =
    rd::typelist_transform<rd::typelist<T...>,
                           rd::data_source_info_header_t>::type;

template <typename... T>
using data_source_list_t =
    rd::typelist_apply<std::variant, with_data_source_info_t<T...>>::type;

static_assert(
    std::same_as<
        data_source_list_t<discourse::discourse_info_t, int>,
        std::variant<rd::data_source_info_header_t<discourse::discourse_info_t>,
                     rd::data_source_info_header_t<int>>>);
} // namespace __details

using data_source_info_t =
    __details::data_source_list_t<discourse::discourse_info_t>;
} // namespace rd
