#include <catch2/catch.hpp>

#define SO_USE_STD_OPERATORS
#include "soser.hpp"
#include <string>
#include <sstream>

struct Structure
{
    int i = 42;
    float f = 10.0;
    std::string str = "hello";

    bool operator== (const Structure& s) const {
        return i == s.i and f == s.f and str == s.str;
    }

    bool operator!= (const Structure& s) const {
        return !(*this == s);
    }

    SERIALIZE_THESE(Structure, i, f, str);
};

TEST_CASE("Deserealization works", "[struct]")
{
    Structure strc;

    std::string str("{\"i\": 0, \"f\": 0.000000, \"str\": \"modified\"}");

    str >> strc;

    REQUIRE(strc.i == 0);
    REQUIRE(strc.f == 0.0);
    REQUIRE(strc.str == "modified");
}

TEST_CASE("Can recover members from string properly", "[deserealize1]")
{
    Structure struct1, struct2;

    std::stringstream ss;

    SECTION("Structure can be equal"){
        REQUIRE(struct1 == struct2);
    }

    struct2.i = 11;
    struct2.f = 20.20;
    struct2.str = "something else";

    SECTION("Structures are different"){
        REQUIRE(struct1 != struct2);
    }
    
    ss << struct2;

    ss.str() >> struct1;

    SECTION("Using deserealization recovers members"){
        REQUIRE(struct1 == struct2);
    }

}
