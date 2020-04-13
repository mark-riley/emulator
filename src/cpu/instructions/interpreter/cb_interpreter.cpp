#include <cpu/LR35902.h>

void LR35902::RLC_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    if (test_bit(value, 7)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    value <<= 1;

    if (get_c_flag()) {
        value = set_bit(value, 0);
    } else {
        value = reset_bit(value, 0);
    }

    if (!value) {
        set_z_flag();
    }  else {
        reset_z_flag();
    }

    set_reg_value(reg, value);

    reset_n_flag();
    reset_h_flag();
}

void LR35902::RRC_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    if (test_bit(value, 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    value >>= 1;

    if (get_c_flag()) {
        value = set_bit(value, 7);
    } else {
        value = reset_bit(value, 7);
    }

    if (!value) {
        set_z_flag();
    }  else {
        reset_z_flag();
    }

    set_reg_value(reg, value);

    reset_n_flag();
    reset_h_flag();
}

void LR35902::RL_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);
    bool old_c = get_c_flag();

    if (test_bit(value, 7)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    value <<= 1;

    if (old_c) {
        value = set_bit(value, 0);
    } else {
        value = reset_bit(value, 0);
    }

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    reset_n_flag();
    reset_h_flag();

    set_reg_value(reg, value);
}

void LR35902::RR_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);
    bool old_c = get_c_flag();

    if (test_bit(value, 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    value >>= 1;

    if (old_c) {
        value = set_bit(value, 7);
    } else {
        value = reset_bit(value, 7);
    }

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    reset_n_flag();
    reset_h_flag();

    set_reg_value(reg, value);
}

void LR35902::SLA_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    if (test_bit(value, 7)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }

    value <<= 1;

    value = reset_bit(value, 0);

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    reset_h_flag();

    set_reg_value(reg, value);
}

void LR35902::SRA_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    bool prev = test_bit(value, 7);
    if (test_bit(value, 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }
    value >>= 1;

    if (prev) {
        value = set_bit(value, 7);
    } else {
        value = reset_bit(value, 7);
    }

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    reset_h_flag();

    set_reg_value(reg, value);
}

void LR35902::SWAP_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);
    uint8_t new_value = ((value >> 4) & 0x0F) | ((value << 4) & 0xF0);
    set_reg_value(reg, new_value);

    if (!new_value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }

    reset_n_flag();
    reset_h_flag();
    reset_c_flag();
}

void LR35902::SRL_m(uint8_t opcode) {
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    if (test_bit(value, 0)) {
        set_c_flag();
    } else {
        reset_c_flag();
    }
    value >>= 1;

    value = reset_bit(value, 7);

    if (!value) {
        set_z_flag();
    } else {
        reset_z_flag();
    }
    reset_n_flag();
    reset_h_flag();

    set_reg_value(reg, value);
}

void LR35902::BIT_b_m(uint8_t opcode) {
    int bit = get_y(opcode);
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    if (test_bit(value, bit)) {
        reset_z_flag();
    } else {
        set_z_flag();
    }
    reset_n_flag();
    set_h_flag();
}

void LR35902::RES_b_m(uint8_t opcode) {
    int bit = get_y(opcode);
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    value = reset_bit(value, bit);

    set_reg_value(reg, value);
}

void LR35902::SET_b_m(uint8_t opcode) {
    int bit = get_y(opcode);
    int reg = get_z(opcode);

    uint8_t value = get_reg_value(reg);

    value = set_bit(value, bit);

    set_reg_value(reg, value);
}