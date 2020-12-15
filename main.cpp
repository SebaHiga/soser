#include <iostream>
#include "sopacker.hpp"

struct Job{
    std::string position;
    int years = 0;
    
public:
    // mutable std::vector<std::string_view> _memberNames;
    // mutable std::vector<std::string> _memberValues;
    // auto _prepare () {
    //     if(_memberNames.size() == 0)
    //         _memberNames = iniNames("position, years");
    //     _memberValues = strArrVal(position, years);
    // }

    // friend strfyH& operator<< (strfyH &ss, Job &p){
    //     p._prepare();
    //     ss << serializeObject(p._memberNames, p._memberValues);
    //     return ss;
    // }

    _PACK_THESE_(Job, position, years);
};

class Person{
public:
    Person(const std::string_view name, const std::string_view lastName, const std::size_t &age) :
        m_name(name),
        m_lastName(lastName),
        m_age(age){
            m_job.position = "default";
            m_job.years = age;
            job.push_back({});
            job.push_back({"Nuclear plant technician", 20});
            job.push_back({"Expert gardener", 2});
            job.push_back({"what", 20});
        }
private:
    std::string m_name;
    std::string m_lastName;
    std::size_t m_age;
    Job m_job;
    std::vector<Job> job;

    _PACK_THESE_(Person, m_name, m_lastName, m_age, m_job, job);
};

int main() {
    Job p;
    strfyH strh, strh2;

    strh << p;
    std::cout << strh.unpack() << '\n';
    // std::stringstream ss;
    // ss << p;
    // std::cout << ss.str() << '\n';
    
    Person per("John", "Doe", 39);

    strh2 << per;

    std::cout << strh2.unpack();

    // std::cout << per << '\n';



    return 0;
}
