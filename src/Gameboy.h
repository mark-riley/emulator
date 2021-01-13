#pragma once

#ifndef EMULATOR_GAMEBOY_H
#define EMULATOR_GAMEBOY_H


#include "Cartridge.h"

class Gameboy {
public:
    explicit Gameboy(Cartridge*);
private:
    Cartridge* cartridge;
};


#endif
