#include <iostream>
#include <vector>
#include <sstream>

#define SO_USE_STD_OPERATORS

#include "soser/soser.hpp"
#include <list>

struct Job {
    std::string m_position = "none";
    int m_years = 0;

    _PACK_THESE_(Job, m_years, m_position);
};

class Person {
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t& age)
        : m_name(name)
        , m_lastName(lastName)
        , m_age(age)
    {
        m_jobs.push_back({ "swimmer", 4 });
    }
    std::string m_name = "none";

public:
    std::string m_lastName;
    std::size_t m_age;
    std::vector<Job> m_jobs;

    _PACK_THESE_(Person, m_name, m_lastName, m_jobs);
};

int main()
{
    Job p;

    soser::load(p, "p.txt");
    std::cout << p << '\n';

    std::string str("{\"years\": 42, \"position\": \"modified\"}");

    str >> p;

    std::cout << p << '\n';

    Person per("John", "Doe", 39);

    std::cout << per << '\n';

    std::string str2("{\"m_name\": \"Juan\", \"m_lastName\": \"Perez\", \"jobs\": [{\"years\": 4, \"position\": \"swimmer\", \"vec\": [1, 2, 3, 4]}, {\"years\": 4, \"position\": \"swimmer\"}]}");

    str2 >> per;

    std::cout << per;

    soser::save(per, "person.json");

    return 0;
}
