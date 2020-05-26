#pragma once

#ifndef EMULATOR_MEMORYBUS_H
#define EMULATOR_MEMORYBUS_H

#include <cstdint>
#include "Cartridge.h"
#include "Timer.h"
#include "Interrupt.h"

#define BOOT_ROM_OFFSET 0x0000
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
public:
    explicit MemoryBus(Cartridge *, Timer *, Interrupt *);
    void load_boot_rom(std::vector<uint8_t>);
    uint8_t read_byte(uint16_t);
    uint16_t read_word(uint16_t);
    void write_word(uint16_t, uint16_t);
    void write_byte(uint16_t, uint8_t);
    void doDMATransfer(uint8_t);

    void increment_LY();
    void reset_LY();
    Timer * timer;
private:
    std::vector<uint8_t> boot_rom; // 0000H - 00FFH
    uint8_t vram[0x2000]; // 8000H - 9FFFH
    uint8_t internal_ram_0[0x1000]; // C000H - CFFFH
    uint8_t internal_ram_x[0x1000]; // D000H - DFFFH
    uint8_t object_attribute_memory[0xA0]; // FE00H - FE9FH
    uint8_t hardware_io[0x80]; // FF00H - FF7FH
    uint8_t zero_page[0x7F]; // FF80H - FFFEH

    Cartridge * cartridge;
    Interrupt * interrupt;
};

#endif //EMULATOR_MEMORYBUS_H
