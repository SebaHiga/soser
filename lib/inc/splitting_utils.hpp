#pragma once

#include <string>
#include <list>
#include <array>
#include <string_view>

namespace sopack {

std::list<std::string_view> getContainerList(const std::string_view& data);

template<size_t N>
auto splitVals(const std::string_view& str){
    std::array<std::string, N> arr;

    size_t index = 0;
    size_t peaker = 0;
    size_t now = 0;
    bool parsed = false;

    while (now < str.length()) {
        // Start of new item
        if (str[now] == ':' and str[now + 1] == ' '){
            parsed = true;
            // Start of object
            if (str[now + 2] == '[' or str[now + 2] == '{'){
                now += 2;
                const char opener = str[now] == '[' ? '[' : '{';
                const char closer = str[now] == '[' ? ']' : '}';
                size_t count = 0;

                peaker = now;
                for (; peaker < str.length(); peaker++){
                    if (str[peaker] == opener) count++;
                    if (str[peaker] == closer) {
                        count--;
                        if (count == 0) break;
                    }
                }
                peaker++;
            }
            // Is normal item or string
            else {
                now += 2;
                peaker = now;

                for (; peaker < str.length(); peaker++) if (str[peaker] == ',') break;
            }
        }

        if (parsed) {
            if (peaker >= str.length()) peaker--;
            parsed = false;
            arr[index] = str.substr(now, peaker - now);
            now = peaker;
            index++;
        }

        now++;
    }

    return arr;
}

template<typename T, typename TT>
auto serializeObject (T& names, TT& vals){
    std::string ss;
    ss.append("{");

    for(std::size_t i = 0; i < names.size()-1; i++){
        ss.append("\"").append(names[i]).append("\"" ).append(": ").append(vals[i]).append(", ");
    }

    ss.append("\"").append(names.back()).append("\"").append(": ").append(vals.back());

    ss.append("}");

    return ss;
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

constexpr const auto argCount(const std::string_view& str) noexcept{
    size_t count = 0;

    for(const auto c : str) if (c == ',') count++;
    count++;
    return count;
}

} // namespace
