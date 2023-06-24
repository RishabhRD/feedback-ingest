#pragma once

#include <chrono>
#include <type_traits>

namespace rd {
namespace detail {
template <class _Tp> struct __is_duration : std::false_type {};

template <class _Rep, class _Period>
struct __is_duration<std::chrono::duration<_Rep, _Period>> : std::true_type {};

template <class _Rep, class _Period>
struct __is_duration<const std::chrono::duration<_Rep, _Period>>
    : std::true_type {};

template <class _Rep, class _Period>
struct __is_duration<volatile std::chrono::duration<_Rep, _Period>>
    : std::true_type {};

template <class _Rep, class _Period>
struct __is_duration<const volatile std::chrono::duration<_Rep, _Period>>
    : std::true_type {};
} // namespace detail

template <typename T>
concept is_duration = detail::__is_duration<T>::value;
} // namespace rd
