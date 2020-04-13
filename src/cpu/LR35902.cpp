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

int LR35902::execute_cycle() {
//     SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "AF: %04X BC: %04X DE: %04X HL: %04X SP: %04X PC: %04X\n", AF, BC, DE, HL, SP, PC);

    int cycles = 0;
    if (halt) {
        cycles += 4;
        return cycles;
    }
    uint8_t current_opcode = fetch_byte();
//    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "opcode: %02X\n", current_opcode);

    if (current_opcode == 0x76) {
        cycles += 4;
        HALT();
        return cycles;
    }

    if (get_x(current_opcode) == 0x01) {
        cycles += 4;
        if (get_z(current_opcode) == 0x6) {
            cycles += 4;
        }
        LD_r_r_(current_opcode);
        return cycles;
    }

    uint16_t old_pc = PC;

    switch (current_opcode) {
        case 0x00:
            cycles += 4;
            NOP(current_opcode);
            break;
        case 0x01:
            cycles += 12;
            LD_BC_nn();
            break;
        case 0x02:
            cycles += 8;
            LD__BC__A();
            break;
        case 0x03:
            cycles += 8;
            INC_ss(current_opcode);
            break;
        case 0x04:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x05:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x06:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x07:
            cycles += 4;
            RLCA();
            break;
        case 0x08:
            cycles += 20;
            LD__nn__SP();
            break;
        case 0x09:
            cycles += 8;
            ADD_HL_BC();
            break;
        case 0x0A:
            cycles += 8;
            LD_A__BC_();
            break;
        case 0x0B:
            cycles += 8;
            DEC_ss(current_opcode);
            break;
        case 0x0C:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x0D:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x0E:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x0F:
            cycles += 4;
            RRCA();
            break;

        case 0x10:
            cycles += 4;
            STOP();
            break;
        case 0x11:
            cycles += 12;
            LD_DE_nn();
            break;
        case 0x12:
            cycles += 8;
            LD__DE__A();
            break;
        case 0x13:
            cycles += 8;
            INC_ss(current_opcode);
            break;
        case 0x14:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x15:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x16:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x17:
            cycles += 4;
            RLA();
            break;
        case 0x18:
            cycles += 12;
            JR_n();
            break;
        case 0x19:
            cycles += 8;
            ADD_HL_DE();
            break;
        case 0x1A:
            cycles += 8;
            LD_A__DE_();
            break;
        case 0x1B:
            cycles += 8;
            DEC_ss(current_opcode);
            break;
        case 0x1C:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x1D:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x1E:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x1F:
            cycles += 4;
            RRA();
            break;

        case 0x20:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 4;
            }
            JR_NZ_nn();
            break;
        case 0x21:
            cycles += 12;
            LD_HL_nn();
            break;
        case 0x22:
            cycles += 8;
            LD__HLI__A();
            break;
        case 0x23:
            cycles += 8;
            INC_ss(current_opcode);
            break;
        case 0x24:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x25:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x26:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x27:
            cycles += 4;
            DAA();
            break;
        case 0x28:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 4;
            }
            JR_Z_nn();
            break;
        case 0x29:
            cycles += 8;
            ADD_HL_HL();
            break;
        case 0x2A:
            cycles += 8;
            LD_A__HLI_();
            break;
        case 0x2B:
            cycles += 8;
            DEC_ss(current_opcode);
            break;
        case 0x2C:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x2D:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x2E:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x2F:
            cycles += 4;
            CPL();
            break;

        case 0x30:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 4;
            }
            JR_NC_nn();
            break;
        case 0x31:
            cycles += 12;
            LD_SP_nn();
            break;
        case 0x32:
            cycles += 8;
            LD__HLD__A();
            break;
        case 0x33:
            cycles += 8;
            INC_ss(current_opcode);
            break;
        case 0x34:
            cycles += 12;
            INC_r(current_opcode);
            break;
        case 0x35:
            cycles += 12;
            DEC_r(current_opcode);
            break;
        case 0x36:
            cycles += 12;
            LD_r_n(current_opcode);
            break;
        case 0x37:
            cycles += 4;
            SCF();
            break;
        case 0x38:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 4;
            }
            JR_C_nn();
            break;
        case 0x39:
            cycles += 8;
            ADD_HL_SP();
            break;
        case 0x3A:
            cycles += 8;
            LD_A__HLD_();
            break;
        case 0x3B:
            cycles += 8;
            DEC_ss(current_opcode);
            break;
        case 0x3C:
            cycles += 4;
            INC_r(current_opcode);
            break;
        case 0x3D:
            cycles += 4;
            DEC_r(current_opcode);
            break;
        case 0x3E:
            cycles += 8;
            LD_r_n(current_opcode);
            break;
        case 0x3F:
            cycles += 4;
            CCF();
            break;

        case 0x86:
            cycles += 4;
        case 0x80 ... 0x85:
        case 0x87:
            cycles += 4;
            ADD_s(current_opcode);
            break;

        case 0x8E:
            cycles += 4;
        case 0x88 ... 0x8D:
        case 0x8F:
            cycles += 4;
            ADC_s(current_opcode);
            break;

        case 0x96:
            cycles += 4;
        case 0x90 ... 0x95:
        case 0x97:
            cycles += 4;
            SUB_s(current_opcode);
            break;

        case 0x9E:
            cycles += 4;
        case 0x98 ... 0x9D:
        case 0x9F:
            cycles += 4;
            SBC_s(current_opcode);
            break;

        case 0xA6:
            cycles += 4;
        case 0xA0 ... 0xA5:
        case 0xA7:
            cycles += 4;
            AND_s(current_opcode);
            break;

        case 0xAE:
            cycles += 4;
        case 0xA8 ... 0xAD:
        case 0xAF:
            cycles += 4;
            XOR_s(current_opcode);
            break;

        case 0xB6:
            cycles += 4;
        case 0xB0 ... 0xB5:
        case 0xB7:
            cycles += 4;
            OR_s(current_opcode);
            break;

        case 0xBE:
            cycles += 4;
        case 0xB8 ... 0xBD:
        case 0xBF:
            cycles += 4;
            CP_s(current_opcode);
            break;

        case 0xC0:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 12;
            }
            RET_NZ();
            break;
        case 0xC1:
            cycles += 12;
            POP_BC();
            break;
        case 0xC2:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 4;
            }
            JP_NZ_nn();
            break;
        case 0xC3:
            cycles += 16;
            JP_nn();
            break;
        case 0xC4:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 8;
            }
            CALL_NZ_nn();
            break;
        case 0xC5:
            cycles += 16;
            PUSH_BC();
            break;
        case 0xC6:
            cycles += 8;
            ADD_A_n();
            break;
        case 0xC7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xC8:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 12;
            }
            RET_Z();
            break;
        case 0xC9:
            cycles += 16;
            RET();
            break;
        case 0xCA:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 4;
            }
            JP_Z_nn();
            break;
        case 0xCB:
            cycles = cb_instruction();
            break;
        case 0xCC:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 12;
            }
            CALL_Z_nn();
            break;
        case 0xCD:
            cycles += 24;
            CALL_nn();
            break;
        case 0xCE:
            cycles += 8;
            ADC_A_n();
            break;
        case 0xCF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xD0:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 12;
            }
            RET_NC();
            break;
        case 0xD1:
            cycles += 12;
            POP_DE();
            break;
        case 0xD2:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 4;
            }
            JP_NC_nn();
            break;
        case 0xD3:
            cycles += 4;
            break;
        case 0xD4:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 12;
            }
            CALL_NC_nn();
            break;
        case 0xD5:
            cycles += 16;
            PUSH_DE();
            break;
        case 0xD6:
            cycles += 8;
            SUB_n();
            break;
        case 0xD7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xD8:
            cycles += 8;
            if (PC != old_pc) {
                cycles += 12;
            }
            RET_C();
            break;
        case 0xD9:
            cycles += 16;
            RETI();
            break;
        case 0xDA:
            cycles += 16;
            if (PC != old_pc) {
                cycles += 4;
            }
            JP_C_nn();
            break;
        case 0xDB:
            cycles += 4;
            break;
        case 0xDC:
            cycles += 12;
            if (PC != old_pc) {
                cycles += 12;
            }
            CALL_C_nn();
            break;
        case 0xDD:
            cycles += 4;
            break;
        case 0xDE:
            cycles += 4;
            SBC_n();
            break;
        case 0xDF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xE0:
            cycles += 12;
            LD__n__A();
            break;
        case 0xE1:
            cycles += 12;
            POP_HL();
            break;
        case 0xE2:
            cycles += 8;
            LD__C__A();
            break;
        case 0xE3:
            cycles += 4;
            break;
        case 0xE4:
            cycles += 4;
            break;
        case 0xE5:
            cycles += 16;
            PUSH_HL();
            break;
        case 0xE6:
            cycles += 8;
            AND_n();
            break;
        case 0xE7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xE8:
            cycles += 16;
            ADD_SP_n();
            break;
        case 0xE9:
            cycles += 4;
            JP__HL_();
            break;
        case 0xEA:
            cycles += 16;
            LD__nn__A();
            break;
        case 0xEB:
            cycles += 4;
            break;
        case 0xEC:
            cycles += 4;
            break;
        case 0xED:
            cycles += 4;
            break;
        case 0xEE:
            cycles += 8;
            XOR_n();
            break;
        case 0xEF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xF0:
            cycles += 12;
            LD_A__n_();
            break;
        case 0xF1:
            cycles += 12;
            POP_AF();
            break;
        case 0xF2:
            cycles += 8;
            LD_A__C_();
            break;
        case 0xF3:
            cycles += 4;
            DI();
            break;
        case 0xF4:
            cycles += 4;
            break;
        case 0xF5:
            cycles += 16;
            PUSH_AF();
            break;
        case 0xF6:
            cycles += 8;
            OR_n();
            break;
        case 0xF7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xF8:
            cycles += 12;
            LDHL_SP_n();
            break;
        case 0xF9:
            cycles += 8;
            LD_SP_HL();
            break;
        case 0xFA:
            cycles += 16;
            LD_A__nn_();
            break;
        case 0xFB:
            cycles += 4;
            EI();
            break;
        case 0xFC:
            cycles += 4;
            break;
        case 0xFD:
            cycles += 4;
            break;
        case 0xFE:
            cycles += 8;
            CP_n();
            break;
        case 0xFF:
            cycles += 16;
            RST(current_opcode);
            break;
        default:
            break;
    }

    return cycles;
}

int LR35902::cb_instruction() {
    int cycles = 0;
    uint8_t current_opcode = fetch_byte();

    if (get_z(current_opcode) == 0x6) {
        cycles += 8;
    }

    switch (current_opcode) {
        case 0x00 ... 0x07:
            cycles += 8;
            RLC_m(current_opcode);
            break;
        case 0x08 ... 0x0F:
            cycles += 8;
            RRC_m(current_opcode);
            break;
        case 0x10 ... 0x17:
            cycles += 8;
            RL_m(current_opcode);
            break;
        case 0x18 ... 0x1F:
            cycles += 8;
            RR_m(current_opcode);
            break;
        case 0x20 ... 0x27:
            cycles += 8;
            SLA_m(current_opcode);
            break;
        case 0x28 ... 0x2F:
            cycles += 8;
            SRA_m(current_opcode);
            break;
        case 0x30 ... 0x37:
            cycles += 8;
            SWAP_m(current_opcode);
            break;
        case 0x38 ... 0x3F:
            cycles += 8;
            SRL_m(current_opcode);
            break;
        case 0x40 ... 0x7F:
            cycles += 8;
            BIT_b_m(current_opcode);
            break;
        case 0x80 ... 0xBF:
            cycles += 8;
            RES_b_m(current_opcode);
            break;
        case 0xC0 ... 0xFF:
            cycles += 8;
            SET_b_m(current_opcode);
            break;
        default:
            break;
    }

    return cycles;
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
