#include <splitting_utils.hpp>
#include <list>

namespace soser {

std::list<std::string_view> getContainerList(const std::string_view& data){
    bool isObjectContainer = data[1] == '{';
    std::list<std::string_view> ret;

    size_t now = 0;
    auto peek = now;
    bool found = false;

    auto isValid = [] (const char c) {
        return c != '[' and c != ']' and c != ',' and c != ' ';
    };

    while (now < data.length()){
        if (isObjectContainer) {
            if (data[now] == '{'){
                found = true;
                size_t count = 0;
                for(peek = now; peek < data.length(); peek++){
                    if (data[peek] == '{') count++;
                    if (data[peek] == '}'){
                        count--;
                        if (count == 0) break;
                    }
                }
                peek++;
            }
        }
        else{
            if (isValid(data[now])){
                found = true;
                for(peek = now; peek < data.length(); peek++){
                    if (data[peek] == ',') break;   
                }
            }
        }    
    
        if (found) {
            found = false;
            if (peek >= data.length()) peek--;
            ret.push_back(data.substr(now, peek - now));
            now = peek;
        }

        now++;
    }

    return ret;
}

} // namespace