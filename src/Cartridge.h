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
private:
    std::vector<uint8_t> rom;
};

#endif //EMULATOR_CARTRIDGE_H
