#include <file-utils.h>
#include <Cartridge.h>
#include "catch.hpp"

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
        WHEN("I read the ROM's header checksum") {
            uint8_t headerChecksum = cart->getHeaderChecksum();
            THEN("The checksum should be equal to 2AH"){
                REQUIRE(headerChecksum == 0x2a);
            }
        }
        WHEN("I generate the ROM's header checksum") {
            uint8_t headerChecksum = cart->generateHeaderChecksum();
            THEN("The checksum should be equal to 2AH"){
                REQUIRE(headerChecksum == 0x2a);
            }
        }
        WHEN("I read the ROM's type") {
            uint8_t cartridgeType = cart->getCartridgeType();
            THEN("The type should be ROM_ONLY"){
                REQUIRE(cartridgeType == cart->ROM_ONLY);
            }
        }
        WHEN("I read the ROM's size") {
            uint8_t romSize = cart->getRomBanks();
            THEN("The size should be NONE"){
                REQUIRE(romSize == cart->NONE);
            }
        }
        WHEN("I read the ROM's RAM size") {
            uint8_t ramSize = cart->getRamBanks();
            THEN("The size should be NO_RAM"){
                REQUIRE(ramSize == cart->NO_RAM);
            }
        }
    }
}

SCENARIO("Cartridge ROM banking", "[cartridge]") {
    GIVEN("I have loaded a simple ROM file") {
        std::string filename = "../tests/res/dummy_rom_only.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new Cartridge(fileBuf);
        WHEN("I read the ROM's first bank") {
            uint8_t first_byte = cart->peek(0);
            THEN("I should get data from the first bank") {
                REQUIRE(first_byte == 's');
            }
        }
        WHEN("I read the ROM's banked area") {
            uint8_t first_byte = cart->peek(0x4000);
            THEN("I should get data from the banked area") {
                REQUIRE(first_byte == '2');
            }
        }
    }
    GIVEN("I have loaded a multibank ROM file") {
        std::string filename = "../tests/res/dummy_mbc1.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new Cartridge(fileBuf);
        WHEN("I read the ROM's first bank") {
            uint8_t first_byte = cart->peek(0);
            THEN("I should get data from the first bank") {
                REQUIRE(first_byte == 's');
            }
        }
        WHEN("I read the ROM's banked area") {
            uint8_t first_byte = cart->peek(0x4000);
            THEN("I should get data from the banked area") {
                REQUIRE(first_byte == '2');
            }
        }
        WHEN("I select the 3rd bank, and read the ROM's banked area") {
            cart->poke(0x2000, 2);
            uint8_t first_byte = cart->peek(0x4000);
            THEN("I should get data from the banked area") {
                REQUIRE(first_byte == '3');
            }
        }
        WHEN("I select bank 0, and read the ROM's banked area") {
            cart->poke(0x2000, 0);
            uint8_t first_byte = cart->peek(0x4000);
            THEN("I should get data from the first banked") {
                REQUIRE(first_byte == '2');
            }
        }
        WHEN("I select bank 4, and read the ROM's banked area") {
            cart->poke(0x2000, 3);
            uint8_t first_byte = cart->peek(0x4000);
            THEN("I should get data from the forth banked") {
                REQUIRE(first_byte == '4');
            }
        }
    }
}

SCENARIO("Cartridge RAM", "[cartridge]") {
    GIVEN("I have loaded a multibank ROM file without RAM") {
        std::string filename = "../tests/res/dummy_mbc1.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new Cartridge(fileBuf);
        WHEN("I try to activate the RAM") {
            cart->poke(0x0000, 0x0a);
            THEN("Nothing should happen") {
                // check that we handle this correctly
            }
        }
        WHEN("I try to read from the RAM") {
            cart->peek(0x0000);
            THEN("Nothing should happen") {
                // check that we handle this correctly
            }
        }
        WHEN("I try to write to the RAM") {
            cart->poke(0x0000, 0x0a);
            THEN("Nothing should happen") {
                // check that we handle this correctly
            }
        }
    }
    GIVEN("I have loaded a multibank ROM file with RAM") {
        std::string filename = "../tests/res/dummy_mbc1_ram.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new Cartridge(fileBuf);
        WHEN("I try to read from the RAM") {
            cart->peek(0x0000);
            THEN("Nothing should happen") {
                // check that we handle this correctly
            }
        }
        WHEN("I try to write to the RAM") {
            cart->poke(0x0000, 0x0a);
            THEN("Nothing should happen") {
                // check that we handle this correctly
            }
        }
    }
    GIVEN("I have loaded a multibank ROM file with RAM") {
        std::string filename = "../tests/res/dummy_mbc1_ram.gb";
        std::vector<uint8_t> fileBuf = readFileToBuffer(filename);
        auto cart = new Cartridge(fileBuf);
        WHEN("I try to activate the RAM") {
            cart->poke(0x0000, 0x0a);
            THEN("RAM should be accessible") {
                // check that we handle this correctly
            }
        }
        WHEN("I try to read from the RAM") {
            cart->peek(0x0a00);
            THEN("I should read the contents of the RAM") {
                // check that we handle this correctly
            }
        }
        WHEN("I try to write to the RAM") {
            cart->poke(0x0a00, 0x0a);
            THEN("The RAM should contain the value we tried to write") {
                // check that we handle this correctly
            }
        }
        WHEN("I select the 2nd bank, and read the ROM's banked RAM area") {
            cart->poke(0x4000, 0x01);
            cart->poke(0x0a00, 0x03);
            uint8_t first_byte = cart->peek(0x0a00);
            THEN("I should get data from the banked area") {
                REQUIRE(first_byte == 0x03);
            }
        }
    }
}