#pragma once

#include <array>
#include <list>
#include <stdexcept>
#include <string_view>

#include "detail.hpp"
#include "file_utils.hpp"
#include "splitting_utils.hpp"

namespace soser {

template <size_t N>
struct SOSer {
public:
    SOSer() { }
    SOSer([[maybe_unused]] size_t memSize) { openBracket(); }
    SOSer([[maybe_unused]] std::array<std::string_view, N> names)
    {
        m_contents = names;
    }

    std::array<std::string_view, N> m_contents;
    std::list<std::string> m_list;
    std::size_t index = 0;
    std::size_t len = 0;
    std::string m_ret = "";

    inline void startObject() { m_ret.append("{"); }
    inline void endObject() { m_ret.append("}"); }
    inline void openBracket() { m_ret.append("["); }
    inline void closeBracket() { m_ret.append("]"); }
    inline void pushSeparator() { m_ret.append(", "); }

    template <typename T>
    constexpr SOSer& operator<<(const T& val)
    {
        if constexpr (detail::is_numeric<T>) {
            push(std::to_string(val));
        } else if constexpr (detail::is_string<T>) {
            push("\"" + std::string(val) + "\"");
        } else if constexpr (detail::has_soser_serialize<T>) {
            push(val._so_serialize());
        } else {
            throw std::logic_error("Not posible to serialize variable");
        }

        return *this;
    }

    template <typename T>
    constexpr SOSer& operator>>(T& val)
    {
        insertValue(val, pop(true));

        return *this;
    }

    template <typename T>
    constexpr void insertValue(T& val, const std::string_view& data)
    {
        if constexpr (detail::is_integral<T>) {
            val = std::stoi(data.data());
        } else if constexpr (detail::is_floating<T>) {
            val = std::stof(data.data());
        } else if constexpr (detail::is_string<T>) {
            val = data.substr(1, data.length() - 2);
        } else if constexpr (detail::has_soser_serialize<T>) {
            val._so_deserialize(data);
        } else {
            throw std::logic_error("Not posible to deserialize value");
        }
    }

    template <typename CNTR>
    SOSer& operator<<(const CNTR& container) requires detail::is_container<CNTR>
    {
        SOSer<0> tmp(1);

        if (!container.empty()) {
            for (std::size_t i = 0; i < container.size() - 1; i++) {
                tmp << container[i];
                tmp.pushSeparator();
            }
            tmp << container[container.size() - 1];
        }
        tmp.closeBracket();

        push(tmp.unpack());
        return *this;
    }

    template <template <class> class CNTR, typename T>
    SOSer& operator>>(CNTR<T>& container) requires detail::is_container<CNTR<T>>
    {
        container.clear();
        const auto dataList = getContainerList(m_contents[index]);

        for (const auto& d : dataList) {
            T tmp;
            insertValue(tmp, d);
            container.emplace_back(std::move(tmp));
        }

        return *this;
    }

    std::string unpack()
    {
        return m_ret;
    }

    template <typename T>
    void push(const T& str)
    {
        if (N > 0) {
            if (index <= N) {
                m_ret.append("\"").append(m_contents[index]).append("\": ").append(str);
                if (index != N - 1) {
                    m_ret.append(", ");
                }
            } else {
                throw std::out_of_range("Cannot insert more object to static "
                                        "array");
            }
        } else {
            m_ret.append(str);
        }
        index++;
        len += str.length() + 2;
    }

    std::string_view pop(const bool view = false)
    {
        std::string_view ret;

        if (view and index <= N) {
            ret = m_contents[index];
        } else {
            if (N > 0 and index <= N) {
                ret = m_contents[index];
            } else {
                if (m_list.size() > 0) {
                    auto tmp = m_list.front();
                    m_list.pop_front();
                    ret = tmp;
                } else {
                    throw std::out_of_range("No items left from the dynamic "
                                            "array");
                }
            }
        }
        index++;
        return ret;
    }
};

template <size_t N, class... T>
std::string serializeObject(const std::array<std::string_view, N> names, const T&... args)
{
    SOSer<N> strHelper(names);
    strHelper.startObject();

    (strHelper << ... << args);

    strHelper.endObject();
    return strHelper.unpack();
}

template <size_t N, typename... T>
auto deSerializeObject(const std::string_view& content, T&... args)
{
    SOSer<N> strHelper(splitVals<N>(content));

    (strHelper >> ... >> args);
}

} // namespace soser

#ifdef SO_USE_STD_OPERATORS
#define _PACK_THESE_(TYPE, ...)                                                                     \
    static constexpr std::array<std::string_view, soser::argCount(#__VA_ARGS__)>                    \
        _so_memberNames { soser::iniNames<soser::argCount(#__VA_ARGS__)>(                           \
            #__VA_ARGS__) };                                                                        \
                                                                                                    \
public:                                                                                             \
    std::string _so_serialize() const                                                               \
    {                                                                                               \
        return soser::serializeObject<soser::argCount(#__VA_ARGS__)>(_so_memberNames, __VA_ARGS__); \
    }                                                                                               \
    void _so_deserialize(const std::string_view& data)                                              \
    {                                                                                               \
        soser::deSerializeObject<soser::argCount(#__VA_ARGS__)>(data,                               \
            __VA_ARGS__);                                                                           \
    }                                                                                               \
    friend std::string operator>>(const std::string& data, TYPE& t)                                 \
    {                                                                                               \
        t._so_deserialize(data);                                                                    \
        return data;                                                                                \
    }                                                                                               \
    template <soser::detail::not_soser_helper T>                                                    \
    friend T& operator<<(T& os, const TYPE& t)                                                      \
    {                                                                                               \
        os << t._so_serialize();                                                                    \
        return os;                                                                                  \
    }
#else

#define _PACK_THESE_(TYPE, ...)                                                  \
    static constexpr std::array<std::string_view, soser::argCount(#__VA_ARGS__)> \
        _so_memberNames { soser::iniNames<soser::argCount(#__VA_ARGS__)>(        \
            #__VA_ARGS__) };                                                     \
                                                                                 \
public:                                                                          \
    void _so_deserialize(const std::string_view& data)                           \
    {                                                                            \
        soser::deSerializeObject<soser::argCount(#__VA_ARGS__)>(data,            \
            __VA_ARGS__);                                                        \
    }                                                                            \
    decltype(auto) _so_serialize() const                                         \
    {                                                                            \
        return soser::serializeObject<soser::argCount(#__VA_ARGS__)>(            \
            _so_memberNames,                                                     \
            soser::serializeObject<soser::argCount(#__VA_ARGS__)>(__VA_ARGS__)); \
    }
#endif
