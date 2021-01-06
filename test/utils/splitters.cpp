#include "catch.hpp"
#include <string>
#include <sstream>

#define SO_USE_STD_OPERATORS
#include "splitting_utils.hpp"

#define STRING_1 "{\"array\": [1, 2, 3], \"color\": \"gold\", \"null\": \"null\", \"number\": 123, \"object\": {\"a\": \"b\", \"c\": \"d\"}, \"string\": \"Hello World\"}"

#define STRING_2 "[{\"years\": 4, \"position\": \"swimmer\", \"vec\": [1, 2, 3, 4]}, {\"years\": 4, \"position\": \"swimmer\", \"vec\": [1, 2, 3, 4]}]"

TEST_CASE("Testing value splitter with simple case", "[splitVals]")
{
    std::string str(STRING_1);

    auto list = soser::splitVals<6>(str);

    std::list<std::string_view> expected{
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

TEST_CASE("Array objects", "[ObjArray]"){
    std::string str(STRING_2);

    auto list = soser::getContainerList(str);

    std::list<std::string_view> expected{
        "{\"years\": 4, \"position\": \"swimmer\", \"vec\": [1, 2, 3, 4]}",
        "{\"years\": 4, \"position\": \"swimmer\", \"vec\": [1, 2, 3, 4]}"
    };

    SECTION("Values must be equal"){
        auto itList = list.begin();
        auto itExpected = expected.begin();
        for(; itList != list.end(); itList++, itExpected++){
            REQUIRE( *itList == *itExpected );
        }
    }
}