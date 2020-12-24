#include <iostream>
#include <list>
#include <string_view>
#include <array>
#include <sstream>
#include "splitting_utils.hpp"

namespace sopack{

namespace detail{

template<typename T> concept is_integral = std::is_integral<T>::value;
template<typename T> concept is_floating = std::is_floating_point<T>::value;
template<typename T> concept is_numeric = is_integral<T> || is_floating<T>;
template<typename T> concept is_char_array = std::is_same<T, const char*>::value;
template<typename T> concept is_char = std::is_same<T, const char>::value;
template<typename T> concept is_string_class = requires (T str) {str.substr();};
template<typename T> concept is_string = is_char_array<T> || is_char<T> || is_string_class<T>;
template<typename T> concept has_sop_serialize = requires (T data) {data._so_serialize();};
template<typename T> concept has_begin = requires (T data) {data.begin();};
template<typename T> concept is_container = has_begin<T> && !is_string<T>;
template<typename T> concept is_so_helper = requires (T data) {data.openBracket();};
template<typename T> concept not_so_helper = !is_so_helper<T>;

} // namespace

template<size_t N>
struct packHelper{
public:
    packHelper(){}
    packHelper([[maybe_unused]] size_t memSize){ openBracket(); }
    packHelper([[maybe_unused]] std::array<std::string, N> content){ arr = content; }
    std::array<std::string, N> arr;
    std::list<std::string> list;
    std::size_t index = 0;

    inline void openBracket() { list.push_back("["); }
    inline void closeBracket() { list.push_back("]"); }

    template<typename T>
    constexpr packHelper& operator<< (const T& val){
        if constexpr (detail::is_numeric<T>){
            push(std::to_string(val));
        }
        else if constexpr (detail::is_string<T>){
            push("\"" + std::string(val) + "\"");
        }
        else if constexpr (detail::has_sop_serialize<T>){
            push(val._so_serialize());
        }

        return *this;
    }

    template<typename T>
    constexpr packHelper& operator>> (T& val){
        insertValue(val, pop());

        index++;

        return *this;
    }

    template<typename T>
    constexpr void insertValue (T& val, std::string data){
        if constexpr (detail::is_integral<T>){
            val = std::stoi(data);
        }
        else if constexpr (detail::is_floating<T>){
            val = std::stof(data);
        }
        else if constexpr (detail::is_string<T>){
            val = data.substr(1, arr[index].length()-2);
        }
        else if constexpr (detail::has_sop_serialize<T>){
            val._so_deserialize(data);
        }
    }

    template<typename CNTR>
    packHelper& operator<< (const CNTR& container) requires detail::is_container<CNTR>
    {
        packHelper<0> tmp(1);

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                tmp << *it ;
                tmp.pushSeparator();
            }
        }

        push(tmp.unpack());
        return *this;
    }

    template<typename T>
    packHelper& operator>> (std::vector<T>& container)
    {
        container.clear();
        const auto& data = arr[index];
        auto dataList = getContainerList(data);

        for(const auto& d : dataList){
            T tmp;

            insertValue(tmp, d);

            container.push_back(tmp);
        }

        index++;
        return *this;
    }

    std::string unpack(){
        std::string ss;

        if (N <= 0){
            list.pop_back(); 
            closeBracket();
            for(const auto& data : list) ss.append(data);
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
            list.emplace_back(str);
        }
        index++;
    }

    std::string pop(){
        if (N > 0) {
            if (index < N){
                return arr[index];
            }
            else{
                std::cout << "cant push more objects\n";
            }
        }
        else{
            auto tmp = list.front();
            list.pop_front();
            return tmp;
        }
        index++;
        return "";
    }

    void pushSeparator(){
        list.emplace_back(", ");
    }
};

template<size_t N, class... T>
auto toStrArr(T& ...args){
    packHelper<N> strHelper(sizeof...(T));

    (strHelper << ... << args);

    return std::move(strHelper.arr);
}

template<size_t N, typename... T>
auto deSerializeObject(const std::string &content, T& ...args){
    auto arr = splitVals<N>(content);
    packHelper<N> strHelper(arr);

    (strHelper >> ... >> args);
}

} // namespace


#ifdef SO_USE_STD_OPERATORS
#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, sopack::argCount(#__VA_ARGS__)> _so_memberNames{sopack::iniNames<sopack::argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
mutable std::array<std::string, sopack::argCount(#__VA_ARGS__)> _so_memberValues;\
public:\
decltype(auto) _so_serialize () const {\
_so_memberValues = sopack::toStrArr<sopack::argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return sopack::serializeObject(_so_memberNames, _so_memberValues);\
 }\
 void _so_deserialize (const std::string& data){\
     sopack::deSerializeObject<sopack::argCount(#__VA_ARGS__)>(data, __VA_ARGS__);\
 }\
friend std::string operator>> (const std::string& data, TYPE& t){\
    t._so_deserialize(data);\
    return data;\
}\
template<sopack::detail::not_so_helper T>\
friend T& operator<< (T& os, const TYPE& t)\
{ os << (sopack::packHelper<1>() << t).unpack(); return os; }
#else 

#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, sopack::argCount(#__VA_ARGS__)> _so_memberNames{sopack::iniNames<sopack::argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
mutable std::array<std::string, sopack::argCount(#__VA_ARGS__)> _so_memberValues;\
public:\
decltype(auto) _so_serialize () const {\
_so_memberValues = sopack::toStrArr<sopack::argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return sopack::serializeObject(_so_memberNames, _so_memberValues);\
 }
#endif
