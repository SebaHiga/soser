# SOSER: Small Object Serializer
SOSer is a object serializer, probably not the best, but quite easy to use! It includes standard options to serialize or deserialize objects with std::fstream or std::string.

## Another serializer, really?
Many serializers have better features like encoding or object binarization. Even if it's a nice to have feature, I was not interested on those (yet). There is a interesting library like this one named [Tser](https://github.com/KonanM/tser) and I my inspiration to create this library comes from there! 

# Dependencies
- Nothing

# Compiler support versions (under review)
- c++ > 9.3.0
- gcc > 9.2
- clang > 11

# Design goals
- Output with JSON format.
- Fast? I don't know
- Easy to implement.
- Easy to deimplement.
- Support for STL containers. 
- Hopefully fun to read.
- Readable saved serialized objects.


# A very simple example to print object on console

## Using the SO_USE_STD_OPERATORS flag
```cpp
#include <iostream>

// Let soser to let override STD features
#define SO_USE_STD_OPERATORS
#include "soser.hpp"

struct Job{
    std::string m_position = "none";
    int m_years = 0;

    // Use this macro to define which members to serialize
   _PACK_THESE_(Job, m_years, m_position);
};

int main () {
    Job job;

    std::cout << job;
    // It outputs {"m_position": "none", "m_years": 0}
}
```

## Without the flag
```cpp
#include <iostream>
#include "soser.hpp"

struct Job{
    std::string m_position = "none";
    int m_years = 0;

   _PACK_THESE_(Job, m_years, m_position);
};

int main () {
    Job job;

    std::cout << job._so_serialize();
    // It outputs {"m_position": "none", "m_years": 0}
}
```

## Quick notes for benchmarking
If you want to compile the benchmarks using conan, you need to specify some compile features for boost libraries


```sh
conan profile update settings.compiler.libcxx=libstdc++11 profile
conan install --settings compiler.libcxx="libstdc++11" ..
```
