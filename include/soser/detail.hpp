#pragma once

#include <type_traits>

namespace soser ::detail {

template <typename T, typename = void>
struct is_iterable : std::false_type {
};

template <typename T>
struct is_iterable<T, std::void_t<decltype(std::declval<T>().begin()), decltype(std::declval<T>().end())>>
    : std::true_type {
};

template <typename T, typename = void>
struct has_soser_helper : std::false_type {
};

template <typename T>
struct has_soser_helper<T, std::void_t<decltype(std::declval<T>()._so_serialize())>> : std::true_type {
};

template <typename T>
struct is_string
    : public std::disjunction<
          std::is_same<char*, typename std::decay_t<T>>,
          std::is_same<const char*, typename std::decay_t<T>>,
          std::is_same<std::string, typename std::decay_t<T>>> {
};

template <typename T, typename = void>
struct is_soser_helper : std::false_type {
};

template <typename T>
struct is_soser_helper<T, std::void_t<decltype(std::declval<T>().openBracket())>>
    : std::true_type {
};

} // namespace
