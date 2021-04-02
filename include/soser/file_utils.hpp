#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

#include "soser.hpp"

namespace soser {

template <typename T>
bool load(T& data, const std::string& path)
{
    std::ifstream file(path.c_str(), std::ifstream::in);
    std::string content;
    if (file.is_open()) {
        getline(file, content);
        content >> data;
        return true;
    }
    return false;
}

template <typename T>
bool save(T& data, const std::string& path)
{
    std::ofstream output(path);

    if (output.is_open()) {
        output << data._so_serialize();
        return true;
    }
    return false;
}

} // namespace soser
