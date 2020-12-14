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
        arr.emplace_back(ss.str());
        return *this;
    }

    template<template<class...> class CNTR, typename... T> 
    void containerHelper(CNTR<T...> container){
        ss.str(std::string());
        ss << '[';

        if(!container.empty()){
            for(auto it = container.begin(); it != container.end(); it++){
                ss << *it << ", ";
            }
            ss << container.back();
        }

        ss << ']';
        arr.emplace_back(ss.str());
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

template<typename T>
auto serializeObject (T& thing){
    std::stringstream ss;

    ss << '[';

    auto [names, vals] = thing._pack();
    for(std::size_t i = 0; i < names.size()-1; i++){
        ss << names[i] << '=' << vals[i] << ", ";
    }

    ss << names.back() << '=' << vals.back();

    ss << ']';

    return ss.str();
}


#define _PACK_THESE_(TYPE, AR...)\
public:\
mutable std::vector<std::string_view> _memberNames;\
mutable std::vector<std::string> _memberValues;\
auto _pack () {\
    if(_memberNames.size() == 0)\
        _memberNames = iniNames(#AR);\
    _memberValues = strArrVal(AR);\
    return std::tie(_memberNames, _memberValues);\
}\
template<typename STRM>\
friend STRM& operator<< (STRM &ss, TYPE &p){\
    ss << serializeObject(p);\
    return ss;\
}\
private:
