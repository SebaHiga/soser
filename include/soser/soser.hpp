#pragma once

#include <iostream>
#include <list>
#include <string_view>
#include <array>
#include <sstream>
#include <stdexcept>
#include "detail.hpp"
#include "splitting_utils.hpp"
#include "file_utils.hpp"

namespace soser {

template<size_t N>
struct SOSer{
public:
    SOSer(){}
    SOSer([[maybe_unused]] size_t memSize){ openBracket(); }
    SOSer([[maybe_unused]] std::array<std::string_view, N> content){
        arr_view = content;
    }
    std::array<std::string, N> arr;
    std::array<std::string_view, N> arr_view;
    std::list<std::string> list;
    std::size_t index = 0;
    std::size_t len = 0;

    inline void openBracket() { list.push_back("["); }
    inline void closeBracket() { list.push_back("]"); }

    template<typename T>
    constexpr SOSer& operator<< (const T& val){
        if constexpr (detail::is_numeric<T>){
            push(std::to_string(val));
        }
        else if constexpr (detail::is_string<T>){
            push("\"" + std::string(val) + "\"");
        }
        else if constexpr (detail::has_soser_serialize<T>){
            push(val._so_serialize());
        }
        else{
            std::string str("Not posible to serialize data value type: ");

            str.append(typeid(T).name());

            throw std::logic_error(str);
        }


        return *this;
    }

    template<typename T>
    constexpr SOSer& operator>> (T& val){
        insertValue(val, pop(true));

        return *this;
    }

    template<typename T>
    constexpr void insertValue (T& val, const std::string_view& data){
        if constexpr (detail::is_integral<T>){
            val = std::stoi(data.data());
        }
        else if constexpr (detail::is_floating<T>){
            val = std::stof(data.data());
        }
        else if constexpr (detail::is_string<T>){
            val = data.substr(1, data.length()-2);
        }
        else if constexpr (detail::has_soser_serialize<T>){
            val._so_deserialize(data);
        }
        else{
            std::string str("Not posible to deserialize data value type: ");

            str.append(typeid(T).name());

            throw std::logic_error(str);
        }
    }

    template<typename CNTR>
    SOSer& operator<< (const CNTR& container) requires detail::is_container<CNTR>
    {
        SOSer<0> tmp(1);

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
    SOSer& operator>> (std::vector<T>& container)
    {
        container.clear();
        const auto dataList = getContainerList(arr_view[index]);

        for(const auto& d : dataList){
            T tmp;
            insertValue(tmp, d);
            container.push_back(std::move(tmp));
        }

        return *this;
    }

    std::string unpack(){
        std::string ss;
        ss.reserve(len);

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
            if (index <= N){
                arr[index] = str;
            }
            else{
                throw std::out_of_range("Cannot insert more object to static array");
            }
        }
        else{
            list.emplace_back(str);
        }
        index++;
        len += str.length() + 2;
    }

    std::string_view pop(const bool view = false){
        std::string_view ret;

        if (view and index <= N) {
            ret = arr_view[index];
        }
        else {
            if (N > 0 and index <= N) {
                ret = arr[index];
            }
            else{
                if (list.size() > 0){
                    auto tmp = list.front();
                    list.pop_front();
                    ret = tmp;
                }
                else{
                    throw std::out_of_range("No items left from the dynamic array");
                }
            }
        }
        index++;
        return ret;
    }

    void pushSeparator(){
        list.emplace_back(", ");
    }
};

template<size_t N, class... T>
auto toStrArr(T& ...args){
    SOSer<N> strHelper(sizeof...(T));

    (strHelper << ... << args);

    return std::move(strHelper.arr);
}

template<size_t N, typename... T>
auto deSerializeObject(const std::string_view& content, T& ...args){
    SOSer<N> strHelper(splitVals<N>(content));

    (strHelper >> ... >> args);
}

} // namespace


#ifdef SO_USE_STD_OPERATORS
#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, soser::argCount(#__VA_ARGS__)> _so_memberNames{soser::iniNames<soser::argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
mutable std::array<std::string, soser::argCount(#__VA_ARGS__)> _so_memberValues;\
public:\
decltype(auto) _so_serialize () const {\
_so_memberValues = soser::toStrArr<soser::argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return soser::serializeObject(_so_memberNames, _so_memberValues);\
 }\
 void _so_deserialize (const std::string_view& data)\
 {soser::deSerializeObject<soser::argCount(#__VA_ARGS__)>(data, __VA_ARGS__);}\
friend std::string operator>> (const std::string& data, TYPE& t)\
{t._so_deserialize(data);return data;}\
template<soser::detail::not_soser_helper T>\
friend T& operator<< (T& os, const TYPE& t)\
{ os << (soser::SOSer<1>() << t).unpack(); return os; }
#else 

#define _PACK_THESE_(TYPE,...)\
private:\
std::array<std::string_view, soser::argCount(#__VA_ARGS__)> _so_memberNames{soser::iniNames<soser::argCount(#__VA_ARGS__)>(#__VA_ARGS__)};\
mutable std::array<std::string, soser::argCount(#__VA_ARGS__)> _so_memberValues;\
public:\
decltype(auto) _so_serialize () const {\
_so_memberValues = soser::toStrArr<soser::argCount(#__VA_ARGS__)>(__VA_ARGS__);\
return soser::serializeObject(_so_memberNames, _so_memberValues);\
 }
#endif
