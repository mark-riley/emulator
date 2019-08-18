#pragma once

#ifndef EMULATOR_CARTRIDGE_H
#define EMULATOR_CARTRIDGE_H

#include <cstdint>
#include <string>
#include <vector>

class Cartridge {
public:
    explicit Cartridge(std::vector<uint8_t>&);

    std::vector<uint8_t> get_rom();  // this is temporary until we can implicitly test the rom contents
    uint8_t peek(int);
    uint16_t peek16(int);

    std::string getTitle();
    uint8_t getHeaderChecksum();
    uint8_t generateHeaderChecksum();
    uint8_t getCartridgeType();
    uint8_t getRomBanks();
    uint8_t getRamBanks();

    enum cartridgeType {
        ROM_ONLY,

        MBC1,
        MBC1_RAM,
        MBC1_RAM_BATTERY,

        MBC2 = 5,
        MBC2_BATTERY,

        ROM_RAM = 8,
        ROM_RAM_BATTERY,

        MMM01 = 0x0b,
        MMM01_RAM,
        MMM01_RAM_BATTERY,

        MBC3_TIMER_BATTERY = 0x0f,
        MBC3_TIMER_RAM_BATTERY,
        MBC3,
        MBC3_RAM,
        MBC3_RAM_BATTERY,

        MBC5 = 0x19,
        MBC5_RAM,
        MBC5_RAM_BATTERY,
        MBC5_RUMBLE,
        MBC5_RUMBLE_RAM,
        MBC5_RUMBLE_RAM_BATTERY,

        MBC6 = 0x20,

        MBC7_SENSOR_RUMBLE_RAM_BATTERY = 0x22,

        POCKET_CAMERA = 0xfc,
        BANDAI_TAMA5,
        HuC3,
        HuC1_RAM_BATTERY
    };

    enum romBanks {
        NONE,
        FOUR,
        EIGHT,
        SIXTEEN,
        THIRTY_TWO,
        SIXTY_FOUR,
        ONE_HUNDRED_AND_TWENTY_EIGHT,
        TWO_HUNDRED_AND_FIFTY_SIX,
        FIVE_HUNDRED_AND_TWELVE,

        SEVENTY_TWO = 0x52,
        EIGHTY,
        NINETY_SIX,
    };

    enum ramBanks {
        NO_RAM,
        QUARTER_RAM,
        FULL_RAM,
        FOUR_RAM,
        SIXTEEN_RAM,
        EIGHT_RAM
    };
private:
    std::vector<uint8_t> rom;
};

#endif //EMULATOR_CARTRIDGE_H
