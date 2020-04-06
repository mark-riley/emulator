#include <stdexcept>
#include "Cartridge.h"

Cartridge::Cartridge(std::vector<uint8_t>& game_rom_buffer) {
    rom = game_rom_buffer;
    ram = std::vector<uint8_t>(0x2000, 0x0);
    romBank = 1;
    ramBank = 0;
    ramEnable = false;
}

std::string Cartridge::getTitle() {
    std::string title = std::string(rom.begin() + 0x0134, rom.begin() + 0x0144);
    return title;
}

uint8_t Cartridge::getHeaderChecksum() {
    return peek(0x14d);
}

uint8_t Cartridge::generateHeaderChecksum() {
    uint8_t headerChecksum = 0;
    for (int i = 0x134; i <= 0x14C; i ++) {
        headerChecksum = headerChecksum - peek(i) - 1;
    }
    return headerChecksum;
}

uint8_t Cartridge::getCartridgeType() {
    return peek(0x147);
}

uint8_t Cartridge::getRomBanks() {
    return peek(0x148);
}

uint8_t Cartridge::getRamBanks() {
    return peek(0x149);
}

uint8_t Cartridge::peek(int memory_location) {
    if (memory_location < 0x4000) {
        return rom[memory_location];
    } else if (memory_location < 0x8000) {
        // ROM bank memory
        int current_bank_offset = romBank * 0x4000;
        int location_in_current_bank = memory_location % 0x4000;
        return rom[current_bank_offset + location_in_current_bank];
    } else if (memory_location < 0xa000) {
        // Nothing
    } else if (memory_location < 0xc000) {
        if (ramEnable) {
            // Cartridge RAM bank
            int current_ram_bank_offset = romBank * 0x2000;
            int location_in_current_ram_bank = memory_location % 0x2000;
            return ram[current_ram_bank_offset + location_in_current_ram_bank + 0xa000];
        }
    }
    throw std::runtime_error("Cartridge memory out of range.");
}

uint16_t Cartridge::peek16(int memory_location) {
    uint16_t return_value = peek(memory_location) << 8;
    return_value |= peek(memory_location + 1);
    return return_value;
}

void Cartridge::poke(int memory_location, uint8_t data) {
    if (memory_location < 0x2000) {
        ramEnable = (data & 0x0a) == 0x0a;
    } else if (memory_location < 0x4000) {
        // Set ROM bank
        int newRomBank = data & 0x1f;
        newRomBank = newRomBank == 0 ? newRomBank + 1 : newRomBank;
        romBank = newRomBank;
    } else if (memory_location < 0x6000) {
        // set RAM bank
        if (ramEnable) {
            ramBank = data & 0x03;
        }
    } else if (memory_location < 0xa000) {
        // do nothing
    } else if (memory_location < 0xc000) {
        // Cartridge RAM
        if (ramEnable) {
            int current_ram_bank_offset = romBank * 0x2000;
            int location_in_current_ram_bank = memory_location % 0x2000;
            ram[current_ram_bank_offset + location_in_current_ram_bank + 0xa000] = data;
        }
    }
}
