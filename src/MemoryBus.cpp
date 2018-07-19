#include <cstdio>
#include <cstring>
#include <SDL2/SDL_log.h>
#include "MemoryBus.h"

void MemoryBus::init() {
    // If there is no cartridge, then the cartridge reads as 0xFF
    memset(&rom_bank_0, 0xFF, sizeof(rom_bank_0));
    memset(&rom_bank_x, 0xFF, sizeof(rom_bank_x));
    write_byte(0xFF0F, 0);
    write_byte(0xFFFF, 0);
}

uint8_t MemoryBus::read_byte(uint16_t memory_location) {
    uint8_t return_value;

    if (memory_location < 0x100) {
        if (!(read_byte(0xFF50) & 0x1)) {
            return_value = boot_rom[memory_location - BOOT_ROM_OFFSET];
        } else {
            return_value = rom_bank_0[memory_location - ROM_BANK_0_OFFSET];
        }
    } else if (memory_location < ROM_BANK_X_OFFSET) {
        return_value = rom_bank_0[memory_location - ROM_BANK_0_OFFSET];
    } else if (memory_location < VRAM_OFFSET) {
        return_value = rom_bank_x[memory_location - ROM_BANK_X_OFFSET];
    } else if (memory_location < CARTRIDGE_RAM_OFFSET) {
        return_value = vram[memory_location - VRAM_OFFSET];
    } else if (memory_location < INTERNAL_RAM_0_OFFSET) {
        return_value = cartridge_ram[memory_location - CARTRIDGE_RAM_OFFSET];
    } else if (memory_location < INTERNAL_RAM_X_OFFSET) {
        return_value = internal_ram_0[memory_location - INTERNAL_RAM_0_OFFSET];
    } else if (memory_location < ECHO_RAM_OFFSET) {
        return_value = internal_ram_x[memory_location - INTERNAL_RAM_X_OFFSET];
    } else if (memory_location < OBJECT_ATTRIBUTE_MEMORY_OFFSET) { // Echo RAM - Reserved, Do Not Use
        return_value = echo_ram[memory_location - ECHO_RAM_OFFSET];
    } else if (memory_location < UNUSABLE_MEMORY_OFFSET) {
        return_value = object_attribute_memory[memory_location - OBJECT_ATTRIBUTE_MEMORY_OFFSET];
    } else if (memory_location < HARDWARE_IO_OFFSET) { // Unusable Memory
        return_value = unusable_memory[memory_location - UNUSABLE_MEMORY_OFFSET];
    } else if (memory_location < ZERO_PAGE_OFFSET) {
        return_value = hardware_io[memory_location - HARDWARE_IO_OFFSET];
    } else if (memory_location < INTERRUPT_ENABLE_FLAG_OFFSET) {
        return_value = zero_page[memory_location - ZERO_PAGE_OFFSET];
    } else {
        return_value = interrupt_enable_flag[memory_location - INTERRUPT_ENABLE_FLAG_OFFSET];
    }

    return return_value;
}

uint16_t MemoryBus::read_word(uint16_t memory_location) {
    return read_byte(memory_location) | (read_byte(memory_location + (uint16_t) 1) << 8);
}

void MemoryBus::write_word(uint16_t memory_location, uint16_t word) {
    write_byte(memory_location, (uint8_t) (word & 0xFF));
    write_byte(memory_location + (uint16_t) 1, (uint8_t) (word >> 8));
}

void MemoryBus::write_byte(uint16_t memory_location, uint8_t byte) {
    if (memory_location < VRAM_OFFSET) {
        // ROM - do not write
    } else if (memory_location < CARTRIDGE_RAM_OFFSET) {
        vram[memory_location - VRAM_OFFSET] = byte;
    } else if (memory_location < INTERNAL_RAM_0_OFFSET) {
        cartridge_ram[memory_location - CARTRIDGE_RAM_OFFSET] = byte;
    } else if (memory_location < INTERNAL_RAM_X_OFFSET) {
        internal_ram_0[memory_location - INTERNAL_RAM_0_OFFSET] = byte;
    } else if (memory_location < ECHO_RAM_OFFSET) {
        internal_ram_x[memory_location - INTERNAL_RAM_X_OFFSET] = byte;
    } else if (memory_location < OBJECT_ATTRIBUTE_MEMORY_OFFSET) { // Echo RAM - Reserved, Do Not Use
        echo_ram[memory_location - ECHO_RAM_OFFSET] = byte;
        write_byte(memory_location - 0x2000, byte);
    } else if (memory_location < UNUSABLE_MEMORY_OFFSET) {
        echo_ram[memory_location - OBJECT_ATTRIBUTE_MEMORY_OFFSET] = byte;
    } else if (memory_location < HARDWARE_IO_OFFSET) {
        // Unusable Memory - do not write
    } else if (memory_location < ZERO_PAGE_OFFSET) {
        if (memory_location==0xFF04){
            byte = 0;
        }

        if (memory_location==0xFF44){
            byte = 0;
        }

        if (memory_location==0xFF46){
            doDMATransfer(byte);
        }

        hardware_io[memory_location - HARDWARE_IO_OFFSET] = byte;
    } else if (memory_location < INTERRUPT_ENABLE_FLAG_OFFSET) {
        zero_page[memory_location - ZERO_PAGE_OFFSET] = byte;
    } else {
        interrupt_enable_flag[memory_location - INTERRUPT_ENABLE_FLAG_OFFSET] = byte;
    }
}

void MemoryBus::doDMATransfer(uint8_t byte) {
    uint16_t address = byte << 8;
    for (int i = 0; i < 0xA0; i++) {
        write_byte(0xFE00 + i, read_byte(address + i));
    }
}

void MemoryBus::load_boot_rom(uint8_t *boot_rom_buffer) {
    memcpy(&boot_rom, boot_rom_buffer, 0x100);
}

void MemoryBus::load_game_rom(uint8_t *game_rom_buffer) {
    memcpy(&rom_bank_0, game_rom_buffer, 0x4000);
    memcpy(&rom_bank_x, game_rom_buffer + 0x4000, 0x4000);
}

void MemoryBus::increment_DIV() {
    ++hardware_io[0xFF04 - HARDWARE_IO_OFFSET];
}

void MemoryBus::increment_LY() {
    ++hardware_io[0xFF44 - HARDWARE_IO_OFFSET];
}

void MemoryBus::reset_LY() {
    hardware_io[0xFF44 - HARDWARE_IO_OFFSET] = 0;
}
