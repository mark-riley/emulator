#include <cstdio>
#include <cstring>
#include <SDL_log.h>
#include "MemoryBus.h"

MemoryBus::MemoryBus(Cartridge * cart, Timer * system_timer, Interrupt * i) {
    // If there is no cartridge, then the cartridge reads as 0xFF
    cartridge = cart;
    timer = system_timer;
    interrupt = i;
}

uint8_t MemoryBus::read_byte(uint16_t memory_location) {
    if (memory_location < 0x100) {
        if (!(read_byte(0xFF50) & 0x1)) {
            return boot_rom[memory_location - BOOT_ROM_OFFSET];
        } else {
            return cartridge->peek(memory_location);
        }
    } else if (memory_location < ROM_BANK_X_OFFSET) {
        return cartridge->peek(memory_location);
    } else if (memory_location < VRAM_OFFSET) {
        return cartridge->peek(memory_location);
    } else if (memory_location < CARTRIDGE_RAM_OFFSET) {
        return vram[memory_location - VRAM_OFFSET];
    } else if (memory_location < INTERNAL_RAM_0_OFFSET) {
        return cartridge->peek(memory_location);
    } else if (memory_location < INTERNAL_RAM_X_OFFSET) {
        return internal_ram_0[memory_location - INTERNAL_RAM_0_OFFSET];
    } else if (memory_location < ECHO_RAM_OFFSET) {
        return internal_ram_x[memory_location - INTERNAL_RAM_X_OFFSET];
    } else if (memory_location < OBJECT_ATTRIBUTE_MEMORY_OFFSET) { // Echo RAM - Reserved, Do Not Use
        return read_byte(memory_location - 0x2000);
    } else if (memory_location < UNUSABLE_MEMORY_OFFSET) {
        return object_attribute_memory[memory_location - OBJECT_ATTRIBUTE_MEMORY_OFFSET];
    } else if (memory_location < HARDWARE_IO_OFFSET) {
        // Unusable Memory
    } else if (memory_location < ZERO_PAGE_OFFSET) {
        if (memory_location==0xFF04){
            return timer->getDIV();
        }

        if (memory_location==0xFF05){
            return timer->getTIMA();
        }

        if (memory_location==0xFF06){
            return timer->getTMA();
        }

        if (memory_location==0xFF07){
            return timer->getTAC();
        }

        if (memory_location==0xFF0F){
            return interrupt->getIF();
        }
        return hardware_io[memory_location - HARDWARE_IO_OFFSET];
    } else if (memory_location < INTERRUPT_ENABLE_FLAG_OFFSET) {
        return zero_page[memory_location - ZERO_PAGE_OFFSET];
    } else {
        if (memory_location == 0xFFFF)
            return interrupt->getIE();
    }
    return 0;
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
        cartridge->poke(memory_location, byte);
    } else if (memory_location < CARTRIDGE_RAM_OFFSET) {
        vram[memory_location - VRAM_OFFSET] = byte;
    } else if (memory_location < INTERNAL_RAM_0_OFFSET) {
        cartridge->poke(memory_location, byte);
    } else if (memory_location < INTERNAL_RAM_X_OFFSET) {
        internal_ram_0[memory_location - INTERNAL_RAM_0_OFFSET] = byte;
    } else if (memory_location < ECHO_RAM_OFFSET) {
        internal_ram_x[memory_location - INTERNAL_RAM_X_OFFSET] = byte;
    } else if (memory_location < OBJECT_ATTRIBUTE_MEMORY_OFFSET) { // Echo RAM - Reserved, Do Not Use
        write_byte(memory_location - 0x2000, byte);
    } else if (memory_location < UNUSABLE_MEMORY_OFFSET) {
        object_attribute_memory[memory_location - OBJECT_ATTRIBUTE_MEMORY_OFFSET] = byte;
    } else if (memory_location < HARDWARE_IO_OFFSET) {
        // Unusable Memory - do not write
    } else if (memory_location < ZERO_PAGE_OFFSET) {
        if (memory_location==0xFF04){
            timer->setDIV(byte);
            return;
        }

        if (memory_location==0xFF05){
            timer->setTIMA(byte);
            return;
        }

        if (memory_location==0xFF06){
            timer->setTMA(byte);
            return;
        }

        if (memory_location==0xFF07){
            timer->setTAC(byte);
            return;
        }

        if (memory_location==0xFF0F){
            interrupt->setIF(byte);
            return;
        }

        if (memory_location==0xFF44){
            reset_LY();
            return;
        }

        if (memory_location==0xFF46){
            doDMATransfer(byte);
            return;
        }

        hardware_io[memory_location - HARDWARE_IO_OFFSET] = byte;
    } else if (memory_location < INTERRUPT_ENABLE_FLAG_OFFSET) {
        zero_page[memory_location - ZERO_PAGE_OFFSET] = byte;
    } else {
        if (memory_location==0xFFFF){
            interrupt->setIE(byte);
        }
    }
}

void MemoryBus::doDMATransfer(uint8_t byte) {
    uint16_t address = byte << 8;

    for (int i = 0; i < 0xA0; i++) {
        write_byte(0xFE00 + i, read_byte(address + i));
    }
}

void MemoryBus::load_boot_rom(std::vector<uint8_t> boot_rom_buffer) {
    boot_rom = boot_rom_buffer;
}

void MemoryBus::increment_LY() {
    if (hardware_io[0xFF44 - HARDWARE_IO_OFFSET] <= 153)
        ++hardware_io[0xFF44 - HARDWARE_IO_OFFSET];
    else
        reset_LY();
}

void MemoryBus::reset_LY() {
    hardware_io[0xFF44 - HARDWARE_IO_OFFSET] = 0;
}
