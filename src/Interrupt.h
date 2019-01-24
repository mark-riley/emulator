#pragma once

#ifndef EMULATOR_INTERRUPT_H
#define EMULATOR_INTERRUPT_H

#include <cstdint>

class Interrupt {
public:
    Interrupt();
    uint16_t do_interrupts();
    void request_interrupt(int);
    bool getIME();
    uint8_t getIF();
    uint8_t getIE();
    void startInterrupts();
    void disableInterrupts();
    void setIE(uint8_t);
    void setIF(uint8_t);
    enum {
        V_BLANK,
        STAT,
        TIMER,
        SERIAL,
        CONTROLLER
    };
private:
    bool start_interrupts;
    bool IME;
    uint8_t IE;
    uint8_t IF;
    bool test_bit(uint8_t, int);
    uint8_t set_bit(uint8_t byte, int bit);
    uint8_t reset_bit(uint8_t, int);
};


#endif //EMULATOR_INTERRUPT_H
