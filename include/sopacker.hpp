#include <iostream>
#include <vector>
#include <string_view>
#include <sstream>
#include <array>
#include <tuple>

namespace sopack{

namespace detail{

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

template<class T>
concept has_begin = requires (T data) {data.begin();};

} // namespace

template<size_t N>
struct packHelper{
public:
    packHelper(){}
    packHelper(size_t memSize){vect.reserve(memSize); openBraket();}
    std::array<std::string, N> arr;
    std::vector<std::string> vect;
    std::size_t index = 0;

    inline void openBraket(){vect.push_back("[");}
    inline void closeBraket(){vect.push_back("]");}

    template<typename T>
    constexpr packHelper& operator<< (const T& val){
        if constexpr (detail::is_integral<T>){
            push(std::to_string(val));
        }
        else if constexpr (detail::is_string<T>){
            push("\"" + std::string(val) + "\"");
        }
        else if constexpr (detail::has_serialize<T>){
            push(val._serialize());
        }
        else{
            std::cout << "cnat deduce wtf: " << typeid(val).name() << '\n';
        }
        return *this;
    }

    std::string unpack(){
        std::string ss;

        if (N <= 0){
            vect.pop_back(); 
            closeBraket();
            for(const auto& data : vect) ss.append(data);
        }
        else{
            for(const auto& data : arr) ss.append(data);
        }

        return ss;
    }

    template<typename T>
    void push(const T& str){
        if (N > 0) {
            if (index < N){
                arr[index] = str;
            }
            else{
                std::cout << "cant push more objects\n";
            }
        }
        else{
            vect.emplace_back(str);
        }
        index++;
    }

    void pushSeparator(){
        vect.emplace_back(", ");
    }

    template<template<class...> class CNTR, typename... T> 
    void containerHelper(const CNTR<T...>& container){
        packHelper<0> tmp(container.size()*2+2);

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                tmp << *it ;
                tmp.pushSeparator();
            }
        }

        push(tmp.unpack());
    }

    template<typename T>
    packHelper& operator<< (const std::vector<T>& container){
        containerHelper(container);
        return *this;
    }

};

template<size_t N, class... T>
auto toStrArr(T& ...args){
    packHelper<N> strHelper(sizeof...(T));

    (strHelper << ... << args);

    return std::move(strHelper.arr);
}

template<size_t N>
constexpr auto splitString(const std::string_view &str, const std::string_view &delim)
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
    } while (pos < str.length() && prev < str.length());
    return tokens;
}

template<size_t N>
constexpr auto iniNames(const std::string_view &names){
    return splitString<N>(names, ", ");
}

template<int N, typename T, typename TT>
auto serializeObject (T& names, TT& vals){
    std::string ss;
    ss.append("{");

    for(std::size_t i = 0; i < names.size()-1; i++){
        ss.append("\"").append(names[i]).append("\"" ).append(" : ").append(vals[i]).append(", ");
    }

    ss.append("\"").append(names.back()).append("\"").append(" : ").append(vals.back());

    ss.append("}");

    return ss;
}

constexpr const auto argCount(const std::string_view& str) noexcept{
    size_t count = 0;

    for(const auto c : str) if (c == ',') count++;
    count++;
    return count;
}

} // namespace


#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, sopack::argCount(#__VA_ARGS__)> _memberNames{sopack::iniNames<sopack::argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
mutable std::array<std::string, sopack::argCount(#__VA_ARGS__)> _memberValues;\
public:\
decltype(auto) _serialize () const {\
_memberValues = sopack::toStrArr<sopack::argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return sopack::serializeObject<sopack::argCount(#__VA_ARGS__)>(_memberNames, _memberValues);\
 }\


