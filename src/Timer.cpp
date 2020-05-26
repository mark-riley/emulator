#include "Timer.h"

bool testBit(uint8_t byte, int bit) {
    return byte >> bit & 1;
}

Timer::Timer(Interrupt * system_interrupt) {
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
    updateDIV(cycles);
    updateTIMA();
}

void Timer::updateDIV(int cycles) {
    DIV += cycles;
}

void Timer::updateTIMA() {
    if (((DIV >> getClockMask()) & 1) && isClockEnabled()) {
        ++TIMA;
        if (TIMA == 0x00) {
            TIMA = TMA;
            interrupt->request_interrupt(interrupt->TIMER);
        }
    }
}

uint8_t Timer::getDIV() {
    return DIV >> 8;
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

uint16_t Timer::getClockMask() {
    switch (getClockFreq()) {
        case 1:
            return 3;
//            return 0x0008; // Increase TIMA every 16 clocks
//            return 0x000F; // Increase TIMA every 16 clocks
        case 2:
            return 5;
//            return 0x0020; // Increase TIMA every 64 clocks
//            return 0x003F; // Increase TIMA every 64 clocks
        case 3:
            return 7;
//            return 0x0080; // Increase TIMA every 256 clocks
//            return 0x00FF; // Increase TIMA every 256 clocks
        case 0:
            return 9;
//            return 0x0200; // Increase TIMA every 1024 clocks
//            return 0x03FF; // Increase TIMA every 1024 clocks
        default:
            return 0;
    }
}

void Timer::setDIV(uint8_t) {
    DIV = 0;
}

void Timer::setTIMA(uint8_t value) {
    TIMA = value;
}

void Timer::setTMA(uint8_t value) {
    TMA = value;
}

void Timer::setTAC(uint8_t value) {
    TAC = value;
}
