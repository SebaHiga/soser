#include <iostream>
#include "sopacker.hpp"

struct Job{
    std::string position;
    int years = 0;
    _PACK_THESE_(Job, position, years)
};

class Person{
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t &age) :
        m_name(name),
        m_lastName(lastName),
        m_age(age){
            m_job.position = "default";
            m_job.years = age;
        }
private:
    std::string m_name;
    std::string m_lastName;
    std::size_t m_age;
    Job m_job;

    _PACK_THESE_(Person, m_name, m_lastName, m_age, m_job);
};

int main() {
    Job p;

    std::stringstream ss;
    ss << p;
    std::cout << ss.str() << '\n';
    
    Person per("John", "Doe", 39);

    std::cout << per << '\n';

    return 0;
}
