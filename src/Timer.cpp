#include "Timer.h"

bool testBit(uint8_t byte, int bit) {
    return byte >> bit & 1;
}

Timer::Timer(Interrupt * system_interrupt) {
    dividerVariable = 0;
    timerVariable = 0;
    clockFreq = 1024;
    interrupt = system_interrupt;
    DIV = 0;
    TIMA = 0;
    TMA = 0;
    TAC = 0;
}

/**
 * DIV FF04 is incremented at a rate of 16384 hz. writing any value resets it to 0.
 *
 * TIMA FF05 is incremented at the rate defined by FF07. When it overflows, it gets reset the the value of FF06 and calls an interrupt ( bit 2 in FF0F, 50)
 *
 * TMA FF06 Nothing special
 *
 * TAC FF07 As mentioned, it defines how fast ff05 increments
 *
 * bit 2 defines if the timers even run (0 = stop, 1= start)
 * bit
 * cycles
 */
void Timer::doTimers(int cycles) {
    dividerVariable += cycles;

    if (dividerVariable > 0xFF) {
        dividerVariable -= 0x100;
        ++DIV;
    }

    if (isClockEnabled()) {
        timerVariable += cycles;
        setClockFreq();

        if (timerVariable >= clockFreq) {
            timerVariable -= clockFreq;
            ++TIMA;

            if (TIMA == 0x00) {
                TIMA = TMA;
                interrupt->request_interrupt(interrupt->TIMER);
            }
        }
    }
}

uint8_t Timer::getDIV() {
    return DIV;
}

uint8_t Timer::getTIMA() {
    return TIMA;
}

uint8_t Timer::getTMA() {
    return TMA;
}

uint8_t Timer::getTAC() {
    return TAC;
}

bool Timer::isClockEnabled() {
    return testBit(TAC, 2);
}

uint8_t Timer::getClockFreq() {
    return TAC & 0x3;
}

void Timer::setClockFreq() {
    switch (getClockFreq()) {
        case 0:
            clockFreq = 1024;  // 4194304 / 4096
            break;
        case 1:
            clockFreq = 16;  // 4194304 / 262144
            break;
        case 2:
            clockFreq = 64;  // 4194304 / 65536
            break;
        case 3:
            clockFreq = 256;  // 4194304 / 16382
            break;
        default:
            break;
    }
}

void Timer::updateDIV(uint8_t value) {
    DIV = 0;
}

void Timer::updateTIMA(uint8_t value) {
    TIMA = value;
}

void Timer::updateTMA(uint8_t value) {
    TMA = value;
}

void Timer::updateTAC(uint8_t value) {
    uint8_t currentFreq = getClockFreq();
    TAC = value;
    uint8_t newFreq = getClockFreq();
    if (currentFreq != newFreq) {
        setClockFreq();
    }
}
