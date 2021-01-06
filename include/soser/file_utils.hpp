#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <string_view>
#include "soser.hpp"

namespace soser {

template<detail::has_soser_serialize T>
void load(T& data, const std::string& path)
{
    std::ifstream file(path.c_str(), std::ifstream::in);
    std::string content;
    if (file.is_open()){
        getline(file, content);
        content >> data;
    }
}

template<detail::has_soser_serialize T>
void save(T& data, const std::string& path)
{
    std::ofstream output(path);
    output << data._so_serialize();
}

} // namespace