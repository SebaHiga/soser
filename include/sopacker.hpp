#include <iostream>
#include <vector>
#include <string_view>
#include <sstream>
#include <array>
#include <tuple>

template <class T>
concept is_integral = std::is_integral<T>::value;

template <class T>
concept is_char_array = std::is_same<T, const char*>::value;

template <class T>
concept is_char = std::is_same<T, const char>::value;

template<class T>
concept is_string_class = requires (T str) {str.substr();};

template<class T>
concept is_string = is_char_array<T> || is_char<T> || is_string_class<T>;

template<class T>
concept has_serialize = requires (T data) {data._serialize();};

template<size_t N>
struct strfyH{
public:
    strfyH(){}
    strfyH(size_t memSize){vect.reserve(memSize); openBraket();}
    std::array<std::string, N> arr;
    std::vector<std::string> vect;
    std::size_t index = 0;

    inline void openBraket(){vect.push_back("[");}
    inline void closeBraket(){vect.push_back("]");}

    template<is_integral T>
    strfyH& operator<< (const T& val){
        push((std::to_string(val)));
        return *this;
    }

    template<is_string T>
    strfyH& operator<< (const T& val){
        push(std::string(val));
        return *this;
    }

    template<has_serialize T>
    strfyH& operator<< (T& val){
        push(val._serialize());
        return *this;
    }

    std::string unpack(){
        std::stringstream ss;

        if (vect.size() > 0){
            closeBraket();
            for(const auto& data : vect) ss << data;
        }
        else{
            for(const auto& data : arr) ss << data;
        }

        return ss.str();
    }

    template<typename T>
    void push(const T& str){
        if (N > 0) {
            arr[index] = str;
        }
        else{
            vect.emplace_back(str);
        }
        index++;
    }

    template<template<class...> class CNTR, typename... T> 
    void containerHelper(CNTR<T...> container){
        strfyH<0> tmp(container.size());
        const char *separator = "";

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                tmp << separator << *it ;
                separator = ", ";
            }
        }

        push(tmp.unpack());
    }

    template<typename T>
    strfyH& operator<< (std::vector<T>& container){
        containerHelper(container);
        return *this;
    }

};

template<size_t N, class... T>
auto strArrVal(T& ...args){
    strfyH<N> strHelper(sizeof...(T));

    (strHelper << ... << args);

    return std::move(strHelper.arr);
}

template<size_t N>
constexpr auto split(const std::string_view &str, const std::string_view &delim)
{
    std::array<std::string_view, N> tokens;
    size_t prev = 0, pos = 0, index = 0;

    do{
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        auto token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens[index] = token;
        index++;
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

template<size_t N>
constexpr auto iniNames(const std::string_view &names){
    return split<N>(names, ", ");
}

template<int N, typename T, typename TT>
auto serializeObject (T& names, TT& vals){
    std::stringstream ss;
    ss << '[';

    for(std::size_t i = 0; i < names.size()-1; i++){
        ss << names[i] << '=' << vals[i] << ", ";
    }

    ss << names.back() << '=' << vals.back();

    ss << ']';

    return ss.str();
}

constexpr const auto argCount(const std::string_view& str) noexcept{
    size_t count = 0;

    for(const auto c : str) if (c == ',') count++;
    count++;
    return count;
}


#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, argCount(#__VA_ARGS__)> _memberNames{iniNames<argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
std::array<std::string, argCount(#__VA_ARGS__)> _memberValues;\
public:\
auto _serialize () {\
_memberValues = strArrVal<argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return serializeObject<argCount(#__VA_ARGS__)>(_memberNames, _memberValues);\
 }\


