#include <cstdio>
#include <SDL2/SDL_log.h>
#include "LR35902.h"

LR35902::LR35902(MemoryBus * memory_bus, Interrupt * i, bool skip_bios) {
    if (skip_bios) {
        PC = 0x0100;
        set_AF(0x01B0);
        set_BC(0x0013);
        set_DE(0x00D8);
        set_HL(0x014D);
        set_SP(0xFFFE);
        memory_bus->write_byte(0xFF50, 0x1);
    } else {
        PC = 0;
    }
    memory = memory_bus;
    interrupt = i;
    halt = false;
}

int LR35902::check_interrupts() {
    if (interrupt->getIF())
        halt = false;

    uint16_t is_interrupt = interrupt->do_interrupts();
    if (is_interrupt) {
        PUSH(PC);
        PC = is_interrupt;
        return 36;
    }
    return 0;
}

void LR35902::set_A(uint8_t value) {
    AF.h = value;
}

void LR35902::set_F(uint8_t value) {
    AF.l = value;
    AF.l &= 0xF0;
}

void LR35902::set_B(uint8_t value) {
    BC.h = value;
}

void LR35902::set_C(uint8_t value) {
    BC.l = value;
}

void LR35902::set_D(uint8_t value) {
    DE.h = value;
}

void LR35902::set_E(uint8_t value) {
    DE.l = value;
}

void LR35902::set_H(uint8_t value) {
    HL.h = value;
}

void LR35902::set_L(uint8_t value) {
    HL.l = value;
}

uint8_t LR35902::get_A() {
    return AF.h;
}

uint8_t LR35902::get_F() {
    return AF.l;
}

uint8_t LR35902::get_B() {
    return BC.h;
}

uint8_t LR35902::get_C() {
    return BC.l;
}

uint8_t LR35902::get_D() {
    return DE.h;
}

uint8_t LR35902::get_E() {
    return DE.l;
}

uint8_t LR35902::get_H() {
    return HL.h;
}

uint8_t LR35902::get_L() {
    return HL.l;
}

void LR35902::set_BC(uint16_t word) {
    BC.rp = word;
}

void LR35902::set_AF(uint16_t word) {
    AF.rp = word;
    AF.l &= 0xF0;
}

void LR35902::set_DE(uint16_t word) {
    DE.rp = word;
}

void LR35902::set_HL(uint16_t word) {
    HL.rp = word;
}

void LR35902::set_SP(uint16_t word) {
    SP = word;
}

uint16_t LR35902::get_AF() {
    return AF.rp;
}

uint16_t LR35902::get_BC() {
    return BC.rp;
}

uint16_t LR35902::get_DE() {
    return DE.rp;
}

uint16_t LR35902::get_HL() {
    return HL.rp;
}

uint16_t LR35902::get_SP() {
    return SP;
}

void LR35902::reset_z_flag() {
    set_F(reset_bit(get_F(), 7));
}

void LR35902::reset_n_flag() {
    set_F(reset_bit(get_F(), 6));
}

void LR35902::reset_h_flag() {
    set_F(reset_bit(get_F(), 5));
}

void LR35902::reset_c_flag() {
    set_F(reset_bit(get_F(), 4));
}

bool LR35902::get_z_flag() {
    return test_bit(get_F(), 7);
}

bool LR35902::get_n_flag() {
    return test_bit(get_F(), 6);
}

bool LR35902::get_h_flag() {
    return test_bit(get_F(), 5);
}

bool LR35902::get_c_flag() {
    return test_bit(get_F(), 4);
}

void LR35902::set_z_flag() {
    set_F(set_bit(get_F(), 7));
}

void LR35902::set_n_flag() {
    set_F(set_bit(get_F(), 6));
}

void LR35902::set_h_flag() {
    set_F(set_bit(get_F(), 5));
}

void LR35902::set_c_flag() {
    set_F(set_bit(get_F(), 4));
}

int LR35902::get_x(uint8_t opcode) {
    return opcode >> 6;
}

int LR35902::get_y(uint8_t opcode) {
    return (opcode >> 3) & 7;
}

int LR35902::get_z(uint8_t opcode) {
    return opcode & 7;
}

int LR35902::get_p(uint8_t opcode) {
    return (get_y(opcode) >> 1) & 3;
}

bool LR35902::get_carry(uint8_t x, uint8_t y) {
    return x + y > 0xFF;
}

bool LR35902::get_half_carry(uint8_t x, uint8_t y) {
    return (x & 0x0F) + (y & 0x0F) > 0x0F;
}

bool LR35902::get_borrow(uint8_t x, uint8_t y) {
    return x < y;
}

bool LR35902::get_half_borrow(uint8_t x, uint8_t y) {
    return (x & 0x0F) < (y & 0x0F);
}

void LR35902::PUSH(uint16_t data) {
    memory->write_byte(get_SP() - 1, data >> 8);
    memory->write_byte(get_SP() - 2, data & 0xFF);
    set_SP(get_SP() - 2);
}

uint16_t LR35902::POP() {
    uint16_t data = memory->read_byte(get_SP());
    data |= (memory->read_byte(get_SP() + 1) << 8);
    set_SP(get_SP() + 2);
    return data;
}

bool LR35902::test_bit(uint8_t byte, int bit) {
    return (byte >> bit) & 1;
}

uint8_t LR35902::set_bit(uint8_t byte, int bit) {
    return byte | 1 << bit;
}

uint8_t LR35902::reset_bit(uint8_t byte, int bit) {
    return byte & ~(1 << bit);
}

uint8_t LR35902::fetch_byte() {
    return memory->read_byte(PC++);
}

uint16_t LR35902::fetch_word() {
    return memory->read_byte(PC++) | (memory->read_byte(PC++) << 8);
}

uint16_t LR35902::get_ss_value(int ss) {
    switch (ss) {
        case 0:
            return get_BC();
        case 1:
            return get_DE();
        case 2:
            return get_HL();
        case 3:
            return get_SP();
        default:
            return 0x00;
    }
};

void LR35902::set_ss_value(int ss, uint16_t value) {
    switch (ss) {
        case 0:
            set_BC(value);
            break;
        case 1:
            set_DE(value);
            break;
        case 2:
            set_HL(value);
            break;
        case 3:
            set_SP(value);
            break;
        default:
            break;
    }
};

uint8_t LR35902::get_reg_value(int reg) {
    switch (reg) {
        case 0:
            return get_B();
        case 1:
            return get_C();
        case 2:
            return get_D();
        case 3:
            return get_E();
        case 4:
            return get_H();
        case 5:
            return get_L();
        case 6:
            return memory->read_byte(get_HL());
        case 7:
            return get_A();
        default:
            return 0x00;
    }
};

void LR35902::set_reg_value(int reg, uint8_t value) {
    switch (reg) {
        case 0:
            set_B(value);
            break;
        case 1:
            set_C(value);
            break;
        case 2:
            set_D(value);
            break;
        case 3:
            set_E(value);
            break;
        case 4:
            set_H(value);
            break;
        case 5:
            set_L(value);
            break;
        case 6:
            memory->write_byte(get_HL(), value);
            break;
        case 7:
            set_A(value);
            break;
        default:
            break;
    }
};
