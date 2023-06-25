#pragma once

namespace rd {
template <typename... List> struct typelist {};

template <template <typename...> class C, typename List> struct typelist_apply;

template <template <typename...> class C, typename... List>
struct typelist_apply<C, typelist<List...>> {
  using type = C<List...>;
};

template <typename List, template <class> class Fun> struct typelist_transform;

template <typename... List, template <class> class Fun>
struct typelist_transform<typelist<List...>, Fun> {
  using type = typelist<Fun<List>...>;
};
} // namespace rd
