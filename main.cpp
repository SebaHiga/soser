#include <iostream>
#include "sopacker.hpp"
#include <fmt/format.h>
struct Job{
    std::string position;
    int years = 0;


//    _PACK_THESE_(Job, position, years);

    std::array<std::string_view, argCount("position, years")> _memberNames{iniNames<argCount("position, years")>("position, years")};
    std::array<std::string, argCount("position, years")> _memberValues;
auto _prepare () {
    _memberValues = strArrVal<argCount("position, years")>(position, years);
}
friend strfyH<argCount("position, years")>& operator<< (strfyH<argCount("position, years")> &ss, Job &p){
    p._prepare();
    ss << serializeObject(p._memberNames, p._memberValues);
    return ss;
}
};

class Person{
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t &age) :
        m_name(name),
        m_lastName(lastName),
        m_age(age){
            jobs.push_back({"Nuclear plant technician", 20});
            jobs.push_back({"Expert gardener", 2});
            jobs.push_back({"Unemployed", 20});
        }
public:
    std::string m_name;
    std::string m_lastName;
    std::size_t m_age;
    std::vector<Job> jobs;
    std::vector<int> m_numbers{1, -2, 4};

    _PACK_THESE_(Person, m_name, m_numbers, jobs);
};

int main() {
    Job p;
    strfyH<argCount("position, years")> strh;
    strfyH<2> strh2;

    strh << p;
    std::cout << strh.unpack() << '\n';
    // // std::stringstream ss;
    // // ss << p;
    // // std::cout << ss.str() << '\n';
    
    Person per("John", "Doe", 39);

    per._prepare();
    std::cout << serializeObject(per._memberNames, per._memberValues);;

    std::cout << strh2.unpack();

    // // std::cout << per << '\n';

    return 0;
}
