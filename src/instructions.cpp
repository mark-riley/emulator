#include "LR35902.h"

int LR35902::execute_cycle() {
    int cycles = 0;
    if (halt) {
        cycles += 4;
        return cycles;
    }
    uint8_t current_opcode = fetch_byte();

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

// Instructions
void LR35902::LD_A__BC_() {
    set_A(memory->read_byte(get_BC()));
}

void LR35902::LD_A__DE_() {
    set_A(memory->read_byte(get_DE()));
}

void LR35902::LD_A__nn_() {
    uint16_t word = fetch_word();
    set_A(memory->read_byte(word));
}

void LR35902::LD__BC__A() {
    memory->write_byte(get_BC(), get_A());
}

void LR35902::LD__DE__A() {
    memory->write_byte(get_DE(), get_A());
}

void LR35902::LD__nn__A() {
    memory->write_byte(fetch_word(), get_A());
}

void LR35902::LD_A__C_() {
    set_A(memory->read_byte(0xFF00 | get_C()));
}

void LR35902::LD__C__A() {
    memory->write_byte(0xFF00 | get_C(), get_A());
}

void LR35902::LD_A__HLD_() {
    set_A(memory->read_byte(get_HL()));
    set_HL(get_HL() - 1);
}

void LR35902::LD__HLD__A() {
    memory->write_byte(get_HL(), get_A());
    set_HL(get_HL() - 1);
}

void LR35902::LD_A__HLI_() {
    set_A(memory->read_byte(get_HL()));
    set_HL(get_HL() + 1);
}

void LR35902::LD__HLI__A() {
    memory->write_byte(get_HL(), get_A());
    set_HL(get_HL() + 1);
}

void LR35902::LD__n__A() {
    uint8_t byte = fetch_byte();
    memory->write_byte(0xFF00 | byte, get_A());
}

void LR35902::LD_A__n_() {
    uint8_t byte = fetch_byte();
    set_A(memory->read_byte(0xFF00 | byte));
}

// 16-bit loads
void LR35902::LD_BC_nn() {
    set_BC(fetch_word());
}

void LR35902::LD_DE_nn() {
    set_DE(fetch_word());
}

void LR35902::LD_HL_nn() {
    set_HL(fetch_word());
}

void LR35902::LD_SP_nn() {
    set_SP(fetch_word());
}

void LR35902::LD_SP_HL() {
    set_SP(get_HL());
}

void LR35902::LDHL_SP_n() {
    int8_t byte = fetch_byte();

    if (((get_SP() ^ byte ^ (get_SP() + byte & 0xFFFF)) & 0x100) == 0x100) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_SP() ^ byte ^ (get_SP() + byte & 0xFFFF)) & 0x10) == 0x10) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_HL(get_SP() + byte);
    reset_z_flag();
    reset_n_flag();
}

void LR35902::LD__nn__SP() {
    memory->write_word(fetch_word(), get_SP());
}

void LR35902::PUSH_AF() {
    PUSH(get_AF());
}

void LR35902::PUSH_BC() {
    PUSH(get_BC());
}

void LR35902::PUSH_DE() {
    PUSH(get_DE());
}

void LR35902::PUSH_HL() {
    PUSH(get_HL());
}

void LR35902::POP_AF() {
    set_AF(POP());
}

void LR35902::POP_BC() {
    set_BC(POP());
}

void LR35902::POP_DE() {
    set_DE(POP());
}

void LR35902::POP_HL() {
    set_HL(POP());
}

void LR35902::ADD_A_n() {
    uint8_t byte = fetch_byte();
    if (get_A() + byte > 0xFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if ((get_A() & 0xF) + (byte & 0xF) > 0xF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_A(get_A() + byte);
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
}

void LR35902::ADC_A_n() {
    uint8_t byte = fetch_byte();
    uint8_t carry = 0;
    if (get_c_flag()) {
        carry = 1;
    }

    if (get_A() + byte + carry > 0xFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if ((get_A() & 0xF) + (byte & 0xF) + carry > 0xF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_A(get_A() + byte + carry);
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
}

void LR35902::SUB_n() {
    uint8_t byte = fetch_byte();
    if (get_A() < byte) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if ((get_A() & 0xF) - (byte & 0xF) < 0) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_A(get_A() - byte);
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    set_n_flag();
}

void LR35902::SBC_n() {
    uint8_t carry = 0;
    if (get_c_flag()) {
        carry = 1;
    }
    uint8_t byte = fetch_byte();
    if (get_A() < (byte + carry)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if ((get_A() & 0xF) - ((byte & 0xF) + carry) < 0) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_A(get_A() - (byte + carry));
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    set_n_flag();
}

void LR35902::AND_n() {
    set_A(get_A() & fetch_byte());
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    set_h_flag();
    reset_c_flag();
}

void LR35902::OR_n() {
    set_A(get_A() | fetch_byte());
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    reset_h_flag();
    reset_c_flag();
}

void LR35902::XOR_n() {
    set_A(get_A() ^ fetch_byte());
    if (!get_A()) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    reset_h_flag();
    reset_c_flag();
}

void LR35902::CP_n() {
    uint8_t byte = fetch_byte();

    if (get_A() == byte) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    if (get_borrow(get_A(), byte)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (get_half_borrow(get_A(), byte)) {
        set_h_flag();
    } else {
        reset_h_flag();
    }
    set_n_flag();
}

// 16-bit Arithmetic
void LR35902::ADD_HL_BC() {
    if (get_HL() + get_BC() > 0xFFFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_HL() & 0xFFF) + (get_BC() & 0xFFF)) > 0xFFF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_HL(get_HL() + get_BC());
    reset_n_flag();
}

void LR35902::ADD_HL_DE() {
    if (get_HL() + get_DE() > 0xFFFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_HL() & 0xFFF) + (get_DE() & 0xFFF)) > 0xFFF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_HL(get_HL() + get_DE());
    reset_n_flag();
}

void LR35902::ADD_HL_HL() {
    if (get_HL() + get_HL() > 0xFFFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_HL() & 0xFFF) + (get_HL() & 0xFFF)) > 0xFFF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_HL(get_HL() + get_HL());
    reset_n_flag();
}

void LR35902::ADD_HL_SP() {
    if (get_HL() + get_SP() > 0xFFFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_HL() & 0xFFF) + (get_SP() & 0xFFF)) > 0xFFF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_HL(get_HL() + get_SP());
    reset_n_flag();
}

void LR35902::ADD_SP_n() {
    int8_t byte = fetch_byte();

    if (((get_SP() ^ byte ^ (get_SP() + byte & 0xFFFF)) & 0x100) == 0x100) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((get_SP() ^ byte ^ (get_SP() + byte & 0xFFFF)) & 0x10) == 0x10) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    set_SP(get_SP() + byte);

    reset_z_flag();
    reset_n_flag();
}

void LR35902::DAA() {
    int byte_1 = get_A();

    if (get_n_flag()) {
        // previous instruction was SUB or SBC
        if (get_h_flag()) {
            byte_1 = (byte_1 - 0x06) & 0xFF;
        }

        if (get_c_flag()) {
            byte_1 -= 0x60;
        }
    } else {
        // previous instruction was ADD or ADC
        if (get_h_flag() || ((byte_1 & 0xF) > 0x09)) {
            byte_1 += 0x06;
        }

        if (get_c_flag() || (byte_1 > 0x9F)) {
            byte_1 += 0x60;
        }
    }

    if (byte_1 & 0x100) {
        set_c_flag();
    }

    set_A(byte_1 & 0xFF);

    reset_h_flag();

    if (get_A() == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

}

void LR35902::CPL() {
    set_A(get_A() ^ 0xFF);
    set_n_flag();
    set_h_flag();
}

void LR35902::CCF() {
    if (get_c_flag()) {
        reset_c_flag();
    } else {
        set_c_flag();
    }

    reset_n_flag();
    reset_h_flag();
}

void LR35902::SCF() {
    set_c_flag();

    reset_n_flag();
    reset_h_flag();
}

void LR35902::HALT() {
    uint8_t interrupt_request_flag = memory->read_byte(0xFF0F);
    uint8_t interrupt_enable_flag = memory->read_byte(0xFFFF);
    uint8_t interrupt_flag = interrupt_request_flag & interrupt_enable_flag;
    if (IME || interrupt_flag)
        halt = true;
    if (!IME && !interrupt_flag)
        halt = true;
}

void LR35902::STOP() {
    halt = true;
}

void LR35902::DI() {
    IME = false;
}

void LR35902::EI() {
    IME = true;
}

// Rotates and Shifts
void LR35902::RLCA() {
    if (test_bit(get_A(), 7)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    set_A(get_A() << 1);

    if (get_c_flag()) {
        set_A(set_bit(get_A(), 0));
    } else {
        set_A(reset_bit(get_A(), 0));
    }

    reset_z_flag();
    reset_n_flag();
    reset_h_flag();

}

void LR35902::RLA() {
    bool old_c = get_c_flag();

    if (test_bit(get_A(), 7)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    set_A(get_A() << 1);

    if (old_c) {
        set_A(set_bit(get_A(), 0));
    } else {
        set_A(reset_bit(get_A(), 0));
    }

    reset_z_flag();
    reset_n_flag();
    reset_h_flag();

}

void LR35902::RRCA() {
    if (test_bit(get_A(), 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    set_A(get_A() >> 1);

    if (get_c_flag()) {
        set_A(set_bit(get_A(), 7));
    } else {
        set_A(reset_bit(get_A(), 7));
    }

    reset_z_flag();
    reset_n_flag();
    reset_h_flag();

}

void LR35902::RRA() {
    bool old_c = get_c_flag();

    if (test_bit(get_A(), 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    set_A(get_A() >> 1);

    if (old_c) {
        set_A(set_bit(get_A(), 7));
    } else {
        set_A(reset_bit(get_A(), 7));
    }

    reset_z_flag();
    reset_n_flag();
    reset_h_flag();

}

// Jumps
void LR35902::JP_nn() {
    PC = fetch_word();
}

void LR35902::JP_NZ_nn() {
    uint16_t dest = fetch_word();

    if (!get_z_flag()) {
        PC = dest;
    }
}

void LR35902::JP_Z_nn() {
    uint16_t dest = fetch_word();

    if (get_z_flag()) {
        PC = dest;
    }
}

void LR35902::JP_NC_nn() {
    uint16_t dest = fetch_word();

    if (!get_c_flag()) {
        PC = dest;
    }
}

void LR35902::JP_C_nn() {
    uint16_t dest = fetch_word();

    if (get_c_flag()) {
        PC = dest;
    }
}

void LR35902::JP__HL_() {
    PC = get_HL();
}

void LR35902::JR_n() {
    PC += (int8_t) fetch_byte();
}

void LR35902::JR_NZ_nn() {
    int8_t dest = fetch_byte();

    if (!get_z_flag()) {
        PC += dest;
    }
}

void LR35902::JR_Z_nn() {
    int8_t dest = fetch_byte();

    if (get_z_flag()) {
        PC += dest;
    }
}

void LR35902::JR_NC_nn() {
    int8_t dest = fetch_byte();

    if (!get_c_flag()) {
        PC += dest;
    }
}

void LR35902::JR_C_nn() {
    int8_t dest = fetch_byte();

    if (get_c_flag()) {
        PC += dest;
    }
}

// Calls
void LR35902::CALL_nn() {
    uint16_t dest = fetch_word();
    PUSH(PC);
    PC = dest;
}

void LR35902::CALL_NZ_nn() {
    uint16_t dest = fetch_word();

    if (!get_z_flag()) {
        PUSH(PC);
        PC = dest;
    }
}

void LR35902::CALL_Z_nn() {
    uint16_t dest = fetch_word();

    if (get_z_flag()) {
        PUSH(PC);
        PC = dest;
    }
}

void LR35902::CALL_NC_nn() {
    uint16_t dest = fetch_word();

    if (!get_c_flag()) {
        PUSH(PC);
        PC = dest;
    }
}

void LR35902::CALL_C_nn() {
    uint16_t dest = fetch_word();

    if (get_c_flag()) {
        PUSH(PC);
        PC = dest;
    }
}

// Returns
void LR35902::RET() {
    PC = POP();
}

void LR35902::RET_NZ() {
    if (!get_z_flag()) {
        PC = POP();
    }
}

void LR35902::RET_Z() {
    if (get_z_flag()) {
        PC = POP();
    }
}

void LR35902::RET_NC() {
    if (!get_c_flag()) {
        PC = POP();
    }
}

void LR35902::RET_C() {
    if (get_c_flag()) {
        PC = POP();
    }
}

void LR35902::RETI() {
    PC = POP();
    IME = true;

}


void LR35902::NOP(uint8_t opcode) {
    // Do nothing
}

void LR35902::LD_r_r_(uint8_t opcode) {
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    reg = get_y(opcode);
    set_reg_value(reg, value);
}

void LR35902::LD_r_n(uint8_t opcode) {
    uint8_t value = fetch_byte();
    int reg = get_y(opcode);
    set_reg_value(reg, value);
}

void LR35902::ADD_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    if (get_carry(accumulator, value)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (get_half_carry(accumulator, value)) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    accumulator += value;

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);

    reset_n_flag();
}

void LR35902::ADC_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);
    int carry = get_c_flag();

    if ((accumulator + value + carry) > 0xFF) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((accumulator & 0xF) + (value & 0xF) + carry) > 0xF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    accumulator += value;
    accumulator += carry;

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);

    reset_n_flag();
}

void LR35902::SUB_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    if (get_borrow(accumulator, value)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (get_half_borrow(accumulator, value)) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    accumulator -= value;

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);

    set_n_flag();
}

void LR35902::SBC_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);
    int carry = get_c_flag();

    if ((accumulator - value - carry) < 0) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (((accumulator & 0xF) - (value & 0xF) - carry) < 0) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    accumulator -= value;
    accumulator -= carry;

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);

    set_n_flag();
}

void LR35902::AND_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    accumulator &= value;

    reset_c_flag();
    set_h_flag();
    reset_n_flag();

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);
}

void LR35902::OR_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    accumulator |= value;

    reset_c_flag();
    reset_h_flag();
    reset_n_flag();

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);
}

void LR35902::XOR_s(uint8_t opcode) {
    uint8_t accumulator = get_A();
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);

    accumulator ^= value;

    reset_c_flag();
    reset_h_flag();
    reset_n_flag();

    if (accumulator == 0) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_A(accumulator);
}

void LR35902::CP_s(uint8_t opcode) {
    int reg = get_z(opcode);
    uint8_t value = get_reg_value(reg);
    if (get_A() == value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    if (get_borrow(get_A(), value)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    if (get_half_borrow(get_A(), value)) {
        set_h_flag();
    } else {
        reset_h_flag();
    }
    set_n_flag();
}

void LR35902::INC_r(uint8_t opcode) {
    int reg = get_y(opcode);
    uint8_t value = get_reg_value(reg);

    ++value;

    if ((value & 0x0F) == 0) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    if (value == 0x00) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    reset_n_flag();

    set_reg_value(reg, value);
}

void LR35902::DEC_r(uint8_t opcode) {
    int reg = get_y(opcode);
    uint8_t value = get_reg_value(reg);

    --value;

    if ((value & 0x0F) == 0xF) {
        set_h_flag();
    } else {
        reset_h_flag();
    }

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    set_n_flag();

    set_reg_value(reg, value);
}

void LR35902::INC_ss(uint8_t opcode) {
    int reg = get_p(opcode);
    uint16_t value = get_ss_value(reg);
    ++value;
    set_ss_value(reg, value);
}

void LR35902::DEC_ss(uint8_t opcode) {
    int reg = get_p(opcode);
    uint16_t value = get_ss_value(reg);
    --value;
    set_ss_value(reg, value);
}

void LR35902::RST(uint8_t opcode) {
    PUSH(PC);
    PC = get_y(opcode) * 8;
}