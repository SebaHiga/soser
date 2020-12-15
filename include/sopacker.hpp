#include <iostream>
#include <vector>
#include <string_view>
#include <sstream>
#include <array>
#include <tuple>

struct strfyH{
public:
    strfyH(){}
    std::stringstream ss;
    std::vector<std::string> arr;
    std::size_t index = 0;

    template<typename T>
    strfyH& operator<< (const T& val){
        ss.str(std::string());
        ss << val;
        auto str = ss.str();
        arr.emplace_back(str);
        return *this;
    }

    std::string unpack(){
        ss.str(std::string());
        for(const auto& data : arr) ss << data;
        return ss.str();
    }

    template<template<class...> class CNTR, typename... T> 
    void containerHelper(CNTR<T...> container){
        strfyH tmp;
        tmp << '[';
        const char *separator = "";

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                tmp << separator << *it ;
                separator = ", ";
            }
        }

        tmp << ']';
        arr.emplace_back(tmp.unpack());
    }

    template<typename T>
    strfyH& operator<< (std::vector<T>& container){
        containerHelper(container);
        return *this;
    }

};

template<class... T>
auto strArrVal(T& ...args){
    strfyH strHelper;
    (strHelper << ... << args);
    return strHelper.arr;
}

auto split(const std::string_view &str, const std::string_view &delim)
{
    std::vector<std::string_view> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        auto token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.emplace_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}

auto iniNames(const std::string_view &names){
    return split(names, ", ");
}

template<typename T, typename TT>
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


#define _PACK_THESE_(TYPE, AR...)\
public:\
    std::vector<std::string_view> _memberNames;\
    std::vector<std::string> _memberValues;\
auto _prepare () {\
    if(_memberNames.size() == 0)\
        _memberNames = iniNames(#AR);\
    _memberValues = strArrVal(AR);\
}\
friend strfyH& operator<< (strfyH &ss, TYPE &p){\
    p._prepare();\
    ss << serializeObject(p._memberNames, p._memberValues);\
    return ss;\
}\
private:
