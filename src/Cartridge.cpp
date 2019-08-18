#include "Cartridge.h"

Cartridge::Cartridge(std::vector<uint8_t>& game_rom_buffer) {
    rom = game_rom_buffer;
}

std::vector<uint8_t> Cartridge::get_rom() {
    return rom;
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
        headerChecksum -= peek(i) - 1;
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
    return rom[memory_location];
}

uint16_t Cartridge::peek16(int memory_location) {
    uint16_t return_value = peek(memory_location) << 8;
    return_value |= peek(memory_location + 1);
    return return_value;
}