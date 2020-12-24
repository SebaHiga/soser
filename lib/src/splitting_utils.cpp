#include <splitting_utils.hpp>
#include <list>

namespace sopack {

std::list<std::string> getContainerList(const std::string& data){
    size_t prev = 0;
    bool isObjectContainer = data[1] == '{';
    int offset = data[prev + 1] == '\"' ? 1 : 0;
    std::list<std::string> ret;

    while (prev < data.length()){
        prev++;
        if (isObjectContainer){
            size_t count = 0;

            // go to the first bracket
            for(const auto& c : data.substr(prev)) if (c != '{') { prev++; } else {break;}
            prev++;
            auto token = prev;

            for(const auto& c : data.substr(prev)){
                if (c == '{') { count++; }
                else if (c == '}'){
                    if (count == 0)  break;
                    count--;
                }
                token++;
            }

            if (token >= data.length() - 1){ token = data.length() - 1; }
            ret.push_back(data.substr(prev, token - prev));
            prev += token - prev + 2;
            if (prev >= data.length() - 1) break;
        }
        else{
            auto token = data.find(',', prev);
            if (token == std::string::npos) { token = data.length() - 1; }
            
            ret.push_back(data.substr(prev + offset, token - prev - 2 * offset)); 
            
            prev += token - prev + 1;
        }
    }

    return ret;
}

} // namespace