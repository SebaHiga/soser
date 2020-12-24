#include "catch.hpp"
#include <string>
#include <sstream>

#define SO_USE_STD_OPERATORS
#include "splitting_utils.hpp"

#define STRING_1 "{\"array\": [1, 2, 3], \"color\": \"gold\", \"null\": \"null\", \"number\": 123, \"object\": {\"a\": \"b\", \"c\": \"d\"}, \"string\": \"Hello World\"}"

TEST_CASE("Testing value splitter", "[splitVals]")
{
    std::string str(STRING_1);

    auto list = sopack::splitVals<7>(str);

    std::list<std::string> expected{
        "[1, 2, 3]",
        "\"gold\"",
        "\"null\"",
        "123",
        "{\"a\": \"b\", \"c\": \"d\"}",
        "\"Hello World\""
    };

    SECTION("Values must be equal"){
        auto itList = list.begin();
        auto itExpected = expected.begin();

        for(; itList != list.end(); itList++, itExpected++){
            REQUIRE( *itList == *itExpected );
        }
    }

}