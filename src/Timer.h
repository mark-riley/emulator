#pragma once

#ifndef EMULATOR_TIMERS_H
#define EMULATOR_TIMERS_H

#include <cstdint>
#include "Interrupt.h"

class Timer {
public:
    explicit Timer(Interrupt*);
    void doTimers(int);
    uint8_t getDIV();
    uint8_t getTIMA();
    uint8_t getTMA();
    uint8_t getTAC();
    void setDIV(uint8_t);
    void setTIMA(uint8_t);
    void setTMA(uint8_t);
    void setTAC(uint8_t);
    void updateTIMA();
    void updateDIV(int);
    uint16_t DIV;

private:
    bool isClockEnabled();
    uint8_t getClockFreq();
    uint16_t getClockMask();

    Interrupt* interrupt;

    uint8_t TIMA;
    uint8_t TMA;
    uint8_t TAC;
};


#endif //EMULATOR_TIMERS_H
