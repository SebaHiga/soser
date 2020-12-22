#include <iostream>
#include "sopacker.hpp"

struct Job{
    std::string position = "none";
    int years = 0;

   _PACK_THESE_(Job, position, years);
};

class Person{
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t &age) :
        m_name(name),
        m_lastName(lastName),
        m_age(age){
        }
public:
    std::string m_name = "none";
    std::string m_lastName;
    std::size_t m_age;
    std::vector<Job> jobs;
    Job j;
    std::vector<int> m_numbers{1, -2, 4};

    _PACK_THESE_(Person, m_name, m_lastName);
};

int main() {
    Job p;
    strfyH<1> strh;
    strfyH<1> strh2;

    // strh << p;
    strh << p;
    std::cout << strh.unpack() << '\n';
    
    // Person per("John", "Doe", 39);

    // strh2 << per;
    // std::cout << strh2.unpack() << '\n';

    return 0;
}
