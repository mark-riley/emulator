#include <iostream>
#include "Cartridge.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

Cartridge::Cartridge(uint8_t * game_rom_buffer) {
    rom = game_rom_buffer;
//    size_t size;
//    switch(ramSize()) {
//        case 0:
//            size = 0;
//            break;
//        case 1:
//            size = 0x0800;
//            break;
//        case 2:
//            size = 0x1000;
//            break;
//        case 3:
//            size = 0x8000;
//            break;
//        default:
//            size = 0;
//            break;
//    }
    currentRomBank = 0;
    currentRamBank = 0;
}

uint8_t Cartridge::peek(uint16_t memory_location) {
    if (memory_location < 0x4000) {
        return rom[memory_location];
    } else if (memory_location < 0x8000) {
        return rom[memory_location + (currentRomBank * 0x4000)];
    } else if (memory_location > 0xA000) {
        return ram[memory_location + (0xA000 * currentRamBank)];
    } else {
        return 0;
    }
}

void Cartridge::poke(uint16_t memory_location, uint8_t value) {
    if (memory_location >= 0x2000 && memory_location < 0x4000) {
        currentRomBank = value;
        if (currentRomBank > 0)
            --currentRomBank;
    } else if (memory_location >= 0x4000 && memory_location < 0x6000) {
        currentRamBank = value % 4;
    } else if (memory_location > 0xA000) {
        ram[memory_location + (0xA000 * currentRamBank)] = value;
    }
}

std::string Cartridge::title() {
    std::string title;
    unsigned long long int size = 16;
    title.assign((char*)this->rom + 0x0134, size);
    return title;
}

std::string Cartridge::manufacturerCode() {
    std::string manufacturerCode;
    unsigned long long int size = 4;
    manufacturerCode.assign((char*)this->rom + 0x013F, size);
    return manufacturerCode;
}

int Cartridge::cbgFlag() {
    return this->rom[0x143];
}

std::string Cartridge::newLicenseeCode() {
    std::string newLicenseeCode;
    unsigned long long int size = 2;
    newLicenseeCode.assign((char*)this->rom + 0x0144, size);
    return newLicenseeCode;
}

int Cartridge::sbgFlag() {
    return this->rom[0x146];
}

int Cartridge::cartridgeType() {
    return this->rom[0x0147];
}

int Cartridge::romSize() {
    return this->rom[0x148];
}

size_t Cartridge::ramSize() {
    return this->rom[0x149];
}

int Cartridge::destinationCode() {
    return this->rom[0x14A];
}

int Cartridge::oldLicenseeCode() {
    return this->rom[0x14B];
}

int Cartridge::maskRomVersion() {
    return this->rom[0x14C];
}
