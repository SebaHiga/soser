#include "catch.hpp"

#define SO_USE_STDCOUT
#include "sopack.hpp"
#include <string>
#include <sstream>

struct Structure
{
    int i = 42;
    float f = 10.0;
    std::string str = "hello";

    _PACK_THESE_(Structure, i, f, str);
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

    REQUIRE(getString(strc) == "{\"i\": 42, \"f\": 10.000000, \"str\": \"hello\"}");
    
    SECTION("Modifying structure is reflected on serialization") {
        strc.str = "world";

        REQUIRE(getString(strc) == "{\"i\": 42, \"f\": 10.000000, \"str\": \"world\"}");
    }
}