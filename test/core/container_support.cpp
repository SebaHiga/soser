#include "catch.hpp"

#define SO_USE_STD_OPERATORS
#include "soser.hpp"
#include <string>
#include <sstream>
#include <vector>

struct VectorInt
{
    std::vector<int> vec_int{1, 2, 3, 4, 5};

    _PACK_THESE_(VectorInt, vec_int);
};

template <typename T>
auto getString(T &data)
{
    std::stringstream ss;

    ss << data;

    return ss.str();
}

TEST_CASE("Vector with integer", "[VectorInt]")
{
    VectorInt strc;

    REQUIRE(getString(strc) == "{\"vec_int\": [1, 2, 3, 4, 5]}");
    
    SECTION("Reverse deserealization") {
        std::string vec_new = "{\"vec_int\": [5, 4, 3, 2, 1]}";

        vec_new >> strc;

        REQUIRE(getString(strc) == vec_new);
    }
}

struct A{
    A() = default;
    A(int _i) : i(_i) {};
    int i = 1;
    _PACK_THESE_(A, i);
};

struct B{
    std::vector<A> vect_A;
    _PACK_THESE_(B, vect_A);
};

TEST_CASE("Container Support with composed structs", "[struct-struct]")
{
    B b;

    b.vect_A.emplace_back(1);
    b.vect_A.emplace_back(2);
    b.vect_A.emplace_back(3);
    b.vect_A.emplace_back(4);
    b.vect_A.emplace_back(5);

    REQUIRE(getString(b) == "{\"vect_A\": [{\"i\": 1}, {\"i\": 2}, {\"i\": 3}, {\"i\": 4}, {\"i\": 5}]}");
}