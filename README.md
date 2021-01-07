# SOSER: Small Object Serializer
SOSer is a object serializer, probably not the best, but quite easy to use! It includes standard options to serialize or deserialize objects with std::fstream or std::string.

## Another serializer, really?
Many serializers have better features like encoding or object binarization. Even if it's a nice to have feature, I was not interested on those (yet). There is a interesting library like this one named [Tser](https://github.com/KonanM/tser) and I my inspiration to create this library comes from there! This library uses Concepts, a new feature from C++20 that greatly improves SFINAE implementations into a simpler sintax.

# Dependencies
- Nothing

# Compiler support versions
- c++ > 9.3.0
- gcc > 9.2

# Design goals
- Easy to implement.
- Easy to deimplement.
- Support for STL containers. 
- Hopefully fun to read.
- Readable saved serialized objects.
- Output with JSON format.


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
}
```
