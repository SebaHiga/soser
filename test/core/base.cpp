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

    SERIALIZE_THESE(Structure, i, f, str);
};

template <typename T>
auto getString(T &data)
{
    std::stringstream ss;

    ss << data;

    return ss.str();
}

TEST_CASE("Basic structure works", "[struct]")
{
    Structure strc;
    std::cout << strc;

    REQUIRE(getString(strc) == "{\"i\": 42, \"f\": 10.000000, \"str\": \"hello\"}");
    
    SECTION("Modifying structure is reflected on serialization") {
        strc.str = "world";

        REQUIRE(getString(strc) == "{\"i\": 42, \"f\": 10.000000, \"str\": \"world\"}");
    }
}

struct A{
    int i = 42;
    SERIALIZE_THESE(A, i);
};

struct B{
    int j = 24;
    A a;
    SERIALIZE_THESE(B, j, a);
};

TEST_CASE("Composition", "[struct-struct]")
{
    B b;

    REQUIRE(getString(b) == "{\"j\": 24, \"a\": {\"i\": 42}}");

    SECTION("It's posible to extract serialization from object member") {
        REQUIRE(getString(b.a) == "{\"i\": 42}");
    }
}
