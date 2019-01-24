#pragma once

#ifndef EMULATOR_CARTRIDGE_H
#define EMULATOR_CARTRIDGE_H

#include <cstdint>
#include <string>

class Cartridge {
public:
    explicit Cartridge(uint8_t *);

    // Queries
    uint8_t peek(uint16_t);
    void poke(uint16_t, uint8_t);

    // Header Queries
    std::string title();
    std::string manufacturerCode();
    int cbgFlag();
    std::string newLicenseeCode();
    int sbgFlag();
    int cartridgeType();
    int romSize();
    size_t ramSize();
    int destinationCode();
    int oldLicenseeCode();
    int maskRomVersion();
private:
    const uint8_t * rom;
    uint8_t * ram;
    int currentRomBank;
    int currentRamBank;
};

#endif //EMULATOR_CARTRIDGE_H
