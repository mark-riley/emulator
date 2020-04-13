#include <cpu/LR35902.h>

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
    uint8_t interrupt_request_flag = interrupt->getIE();
    uint8_t interrupt_enable_flag = interrupt->getIF();
    uint8_t interrupt_flag = interrupt_request_flag & interrupt_enable_flag;
    if (interrupt->getIME() || interrupt_flag)
        halt = true;
    if (!interrupt->getIME() && !interrupt_flag)
        halt = true;
}

void LR35902::STOP() {
    halt = true;
}

void LR35902::DI() {
    interrupt->disableInterrupts();
}

void LR35902::EI() {
    interrupt->startInterrupts();
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
    interrupt->startInterrupts();
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
