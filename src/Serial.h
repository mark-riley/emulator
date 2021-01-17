//
// Created by mark on 16/01/2021.
//

#ifndef EMULATOR_SERIAL_H
#define EMULATOR_SERIAL_H


#include "MemoryBus.h"

class Serial {
public:
    Serial(MemoryBus&);
    MemoryBus& memory;
};


#endif //EMULATOR_SERIAL_H
