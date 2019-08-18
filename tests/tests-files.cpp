#include <file-utils.h>
#include "catch.hpp"

SCENARIO("Reading a file", "[file]") {
    GIVEN("I have a path to a file") {
        std::string filename = "../tests/res/dummy_rom_only.gb";
        WHEN("I pass it to the file reader") {
            std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
            THEN("I get a vector file buffer containing my file contents") {
                std::string test_string = std::string( fileBuf.begin(), fileBuf.begin() + 12 );
                REQUIRE(test_string == "some content");
            }
        }
    }
}