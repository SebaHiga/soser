#include <iostream>
#include <vector>
#include <string_view>
#include <sstream>
#include <array>
#include <tuple>

template<size_t N>
struct strfyH{
public:
    strfyH(){}
    strfyH(size_t memSize){vect.reserve(memSize);}
    std::stringstream ss;
    std::array<std::string, N> arr;
    std::vector<std::string> vect;
    std::size_t index = 0;

    template<typename T>
    strfyH& operator<< (const T& val){
        ss.str(std::string());
        ss << val;
        push(ss.str());        
        return *this;
    }

    std::string unpack(){
        ss.str(std::string());

        if (vect.size() > 0){
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
        tmp << '[';
        const char *separator = "";

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                tmp << separator << *it ;
                separator = ", ";
            }
        }

        tmp << ']';
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

    return strHelper.arr;
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


#define _PACK_THESE_(TYPE, AR...)\
std::array<std::string_view, argCount(#AR)> _memberNames{iniNames<argCount(#AR)>(#AR)};\
std::array<std::string, argCount(#AR)> _memberValues;\
auto _prepare () {_memberValues = strArrVal<argCount(#AR)>(AR);}\
friend strfyH<0>& operator<< (strfyH<0> &ss, TYPE &p){\
p._prepare();\
ss << serializeObject<argCount(#AR)>(p._memberNames, p._memberValues);\
return ss;\
}\
private:
