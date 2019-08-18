#include <file-utils.h>
#include <Cartridge.h>
#include "catch.hpp"

SCENARIO("Loading a ROM", "[cartridge]") {
    GIVEN("I read a ROM file") {
        std::string filename = "../tests/res/dummy_rom_only.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        WHEN("I create a new Cartridge object with the vector contents") {
            auto cart = new  Cartridge(fileBuf);
            THEN("A byte Vector gets populated"){
                std::vector<uint8_t> vec = cart->get_rom();
                std::string test_string = std::string( fileBuf.begin(), fileBuf.begin() + 12 );
                REQUIRE(test_string == "some content");
            }
        }
    }
}

SCENARIO("Reading a ROM", "[cartridge]") {
    GIVEN("I have loaded a ROM file") {
        std::string filename = "../tests/res/dummy_rom_only.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new  Cartridge(fileBuf);
        WHEN("I read the first byte") {
            uint8_t test_byte = cart->peek(0);
            THEN("I see the 's' character"){
                REQUIRE((char)test_byte == 's');
            }
        }
        WHEN("I read the fifth byte") {
            uint8_t test_byte = cart->peek(4);
            THEN("I see a space character"){
                REQUIRE((char)test_byte == ' ');
            }
        }
        WHEN("I read the word starting at memory location 0") {
            uint16_t test_byte = cart->peek16(0);
            THEN("I see an s and an o character"){
                REQUIRE((char)(test_byte >> 8) == 's');
                REQUIRE((char)test_byte == 'o');
            }
        }
        WHEN("I read the word starting at memory location 4") {
            uint16_t test_byte = cart->peek16(4);
            THEN("I see an s and an o character"){
                REQUIRE((char)(test_byte >> 8) == ' ');
                REQUIRE((char)test_byte == 'c');
            }
        }
    }
}

SCENARIO("Cartridge metadata", "[cartridge]") {
    GIVEN("I have loaded a ROM file") {
        std::string filename = "../tests/res/dummy_rom_only.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new  Cartridge(fileBuf);
        WHEN("I read the ROM's title") {
            std::string title = cart->getTitle();
            THEN("The title should be 16 bytes"){
                REQUIRE(title.size() == 16);
            }
            THEN("I see the 'GAME TITLE\0\0\0\0\0\0' string"){
                std::string control_string = "GAME TITLE\0\0\0\0\0\0";
                // TODO this should work without the call to c_str, but control_string is stripping out the NULs
                REQUIRE(title.c_str() == control_string);
            }
        }
        WHEN("I generate the ROM's header checksum") {
            uint8_t headerChecksum = cart->getHeaderChecksum();
            THEN("The checksum should be equal to 5CH"){
                REQUIRE(headerChecksum == 0x5c);
            }
        }
        WHEN("I generate the ROM's header checksum") {
            uint8_t headerChecksum = cart->generateHeaderChecksum();
            THEN("The checksum should be equal to 5CH"){
                REQUIRE(headerChecksum == 0x5c);
            }
        }
    }
}