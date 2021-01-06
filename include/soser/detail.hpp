#pragma once

#include <type_traits>

namespace soser :: detail {

template<typename T> concept is_integral = std::is_integral<T>::value;
template<typename T> concept is_floating = std::is_floating_point<T>::value;
template<typename T> concept is_numeric = is_integral<T> || is_floating<T>;
template<typename T> concept is_char_array = std::is_same<T, const char*>::value;
template<typename T> concept is_char = std::is_same<T, const char>::value;
template<typename T> concept is_string_class = requires (T str) {str.substr();};
template<typename T> concept is_string = is_char_array<T> || is_char<T> || is_string_class<T>;
template<typename T> concept has_soser_serialize = requires (T data) {data._so_serialize();};
template<typename T> concept has_begin = requires (T data) {data.begin();};
template<typename T> concept is_container = has_begin<T> && !is_string<T>;
template<typename T> concept is_soser_helper = requires (T data) {data.openBracket();};
template<typename T> concept not_soser_helper = !is_soser_helper<T>;

} // namespace