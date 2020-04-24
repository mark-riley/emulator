#include "Interrupt.h"
#include <SDL.h>

bool Interrupt::test_bit(uint8_t byte, int bit) {
    return (byte >> bit) & 1;
}

uint8_t Interrupt::set_bit(uint8_t byte, int bit) {
    return byte | (1 << bit);
}

uint8_t Interrupt::reset_bit(uint8_t byte, int bit) {
    return byte & ~(1 << bit);
}

Interrupt::Interrupt() {
    IME = false;
    IF = 0x00;
    IE = 0x00;
}

void Interrupt::request_interrupt(int interrupt) {
    IF = set_bit(IF, interrupt);
}

uint16_t Interrupt::do_interrupts() {
    if (start_interrupts) {
        start_interrupts = false;
        IME = true;
        return 0;
    }

    if (IME) {
        uint8_t interrupt_flag = IF & IE;

        for (int i = 0; i < 5; i++) {
            if (test_bit(interrupt_flag, i)) {
                disableInterrupts();
                IF = reset_bit(IF, i);
                return 0x0040 + (i * 8);
            }
        }
    }
    return 0;
}

bool Interrupt::getIME() {
    return IME;
}

uint8_t Interrupt::getIF() {
    return IF;
}

uint8_t Interrupt::getIE() {
    return IE;
}

void Interrupt::startInterrupts() {
    start_interrupts = true;
}

void Interrupt::disableInterrupts() {
    IME = false;
}

void Interrupt::setIE(uint8_t e) {
    IE = e;
}

void Interrupt::setIF(uint8_t f) {
    IF = f;
}
