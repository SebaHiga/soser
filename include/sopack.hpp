#include <iostream>
#include <list>
#include <string_view>
#include <array>
#include <sstream>

namespace sopack{

namespace detail{

template<typename T> concept is_integral = std::is_integral<T>::value;
template<typename T> concept is_floating = std::is_floating_point<T>::value;
template<typename T> concept is_numeric = is_integral<T> || is_floating<T>;
template<typename T> concept is_char_array = std::is_same<T, const char*>::value;
template<typename T> concept is_char = std::is_same<T, const char>::value;
template<typename T> concept is_string_class = requires (T str) {str.substr();};
template<typename T> concept is_string = is_char_array<T> || is_char<T> || is_string_class<T>;
template<typename T> concept has_so_serialize = requires (T data) {data._so_serialize();};
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
        else if constexpr (detail::has_so_serialize<T>){
            push(val._so_serialize());
        }

        return *this;
    }

    template<typename T>
    constexpr packHelper& operator>> (T& val){
        if constexpr (detail::is_integral<T>){
            val = std::stoi(std::string(arr[index]));
        }
        else if constexpr (detail::is_floating<T>){
            val = std::stof(std::string(arr[index]));
        }
        else if constexpr (detail::is_string<T>){
            val =arr[index].substr(1, arr[index].length()-2);
        }
        else if constexpr (detail::has_so_serialize<T>){
            val._so_deserialize(arr[index]);
        }

        index++;

        return *this;
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

template<size_t N, typename... T>
auto deSerializeObject(const std::string &content, T& ...args){
    auto arr = splitVals<N>(content);
    packHelper<N> strHelper(arr);

    (strHelper >> ... >> args);
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


#ifdef SO_USE_STDCOUT
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
