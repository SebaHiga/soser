#pragma once

#include <string>
#include <list>
#include <array>

namespace sopack {

auto getContainerList(const std::string& data);

template<size_t N>
auto splitVals(const std::string& str){
    std::array<std::string, N> arr;

    auto tmp = str.substr(1);
    size_t index = 0;
    size_t prev = 0;
    size_t token = prev;

    while(index < N){
        prev = tmp.find(": ", prev);
        if (prev != std::string::npos) {
            prev += 2;

            token = tmp.find(',', prev);
            auto bracket = tmp.find('[', prev);
            auto curly_bracket = tmp.find('{', prev);

            if (token == std::string::npos) { token = tmp.length() - 1; }
            if (bracket < token or curly_bracket < token) {
                prev = bracket;
                token = 0;
                
                // find closing bracket
                size_t count = 0;
                char opening = '[', closing = ']';

                if (curly_bracket < bracket){
                    opening = '{'; closing = '}';
                    prev = curly_bracket;
                }

                for (const auto& c : tmp){
                    if (c == opening) count++;
                    if (c == closing){
                        count--;
                        if (!count) break;
                    }
                    token++;
                }
                token++;
            }
            arr[index] = tmp.substr(prev, token - prev);
        }

        index++;
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
