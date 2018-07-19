#pragma once

#ifndef EMULATOR_MEMORYBUS_H
#define EMULATOR_MEMORYBUS_H

#include <cstdint>

#define BOOT_ROM_OFFSET 0x0000
#define ROM_BANK_0_OFFSET 0x0000
#define ROM_BANK_X_OFFSET 0x4000
#define VRAM_OFFSET 0x8000
#define CARTRIDGE_RAM_OFFSET 0xA000
#define INTERNAL_RAM_0_OFFSET 0xC000
#define INTERNAL_RAM_X_OFFSET 0xD000
#define ECHO_RAM_OFFSET 0xE000
#define OBJECT_ATTRIBUTE_MEMORY_OFFSET 0xFE00
#define UNUSABLE_MEMORY_OFFSET 0xFEA0
#define HARDWARE_IO_OFFSET 0xFF00
#define ZERO_PAGE_OFFSET 0xFF80
#define INTERRUPT_ENABLE_FLAG_OFFSET 0xFFFF

class MemoryBus {
private:
    uint8_t boot_rom[0x100]; // 0000H - 00FFH
    uint8_t rom_bank_0[0x4000]; // 0000H - 3FFFH
    uint8_t rom_bank_x[0x4000]; // 4000H - 7FFFH
    uint8_t vram[0x2000]; // 8000H - 9FFFH
    uint8_t cartridge_ram[0x2000]; // A000H - BFFFH
    uint8_t internal_ram_0[0x1000]; // C000H - CFFFH
    uint8_t internal_ram_x[0x1000]; // D000H - DFFFH
    uint8_t echo_ram[0x1E00]; // E000H - FDFFH
    uint8_t object_attribute_memory[0xA0]; // FE00H - FE9FH
    uint8_t unusable_memory[0x60]; // FEA0H - FEFFH
    uint8_t hardware_io[0x80]; // FF00H - FF7FH
    uint8_t zero_page[0x7F]; // FF80H - FFFEH
    uint8_t interrupt_enable_flag[0x1]; // FFFFH - FFFFH
public:
    void init();
    void load_boot_rom(uint8_t *);
    void load_game_rom(uint8_t *);
    uint8_t read_byte(uint16_t);
    uint16_t read_word(uint16_t);
    void write_word(uint16_t, uint16_t);
    void write_byte(uint16_t, uint8_t);
    void doDMATransfer(uint8_t);

    void increment_DIV();

    void increment_LY();
    void reset_LY();
};

#endif //EMULATOR_MEMORYBUS_H
