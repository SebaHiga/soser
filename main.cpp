#include <iostream>
#include <vector>

#define SO_USE_STDCOUT

#include "sopack.hpp"
#include <list>

struct Job{
    Job () = default;
    Job(std::string str, int y) : position(str), years(y) {}
    std::string position = "none";
    int years = 0;
    std::vector<int> vec{1, 2, 3, 4};

   _PACK_THESE_(Job, years, position, vec);
};

class Person{
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t &age) :
        m_name(name),
        m_lastName(lastName),
        m_age(age){
            jobs.emplace_back("swimmer", 4);
        }
    std::string m_name = "none";
public:
    std::string m_lastName;
    std::size_t m_age;
    std::vector<Job> jobs;
    Job working_as;
    std::list<int> m_numbers{1, -2, 4};

    _PACK_THESE_(Person, m_name, m_lastName, jobs);
};

int main() {
    Job p;

    std::cout << p << '\n';    

    std::string str("{\"years\": 42, \"position\": \"modified\", \"vec\": [4, 3, 2, 1]}");

    p._so_deserialize(str);

    std::cout << p << '\n';    

    Person per("John", "Doe", 39);

    std::cout << per << '\n';

    std::string str2("{\"m_name\": \"Sebastian\", \"m_lastName\": \"Modified\", \"jobs\": [{\"years\": 42, \"position\": \"modified\", \"vec\": [4, 4, 4, 4]}, {\"years\": 42, \"position\": \"modified\", \"vec\": [4, 4, 4, 4]}]}");

    per._so_deserialize(str2);

    std::cout << per;

    return 0;
}
