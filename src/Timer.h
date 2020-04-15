#pragma once

#ifndef EMULATOR_TIMERS_H
#define EMULATOR_TIMERS_H

#include <cstdint>
#include "Interrupt.h"

class Timer {
public:
    explicit Timer(Interrupt *);
    void doTimers(int);
    uint8_t getDIV();
    uint8_t getTIMA();
    uint8_t getTMA();
    uint8_t getTAC();
    void updateDIV(uint8_t);
    void updateTIMA(uint8_t);
    void updateTMA(uint8_t);
    void updateTAC(uint8_t);

private:
    bool isClockEnabled();
    uint8_t getClockFreq();
    void setClockFreq();

    Interrupt * interrupt;

    int clockFreq;
    uint16_t DIV;
    uint8_t TIMA;
    uint8_t TMA;
    uint8_t TAC;
};


#endif //EMULATOR_TIMERS_H
