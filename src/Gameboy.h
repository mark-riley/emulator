#pragma once

#ifndef EMULATOR_GAMEBOY_H
#define EMULATOR_GAMEBOY_H


#include <cpu/LR35902.h>
#include "Cartridge.h"
#include "Interrupt.h"
#include "Timer.h"
#include "MemoryBus.h"
#include "Display.h"
#include "Render.h"

class Gameboy {
public:
    explicit Gameboy(Cartridge*, std::vector<uint8_t> romBuf);
    void powerOn();
private:
    Cartridge* cartridge;
    Interrupt* interrupt;
    Timer* timer;
    MemoryBus* mmu;
    LR35902* cpu;
    Display* ppu;
    Render* render;
    Audio* apu;
};


#endif
