#pragma once

#ifndef EMULATOR_Z80_H
#define EMULATOR_Z80_H

#include <cstdint>
#include "RegisterPair.h"
#include "MemoryBus.h"
#include "Interrupt.h"

class LR35902 {
public:
    register_pair AF, BC, DE, HL;

    uint16_t PC;
    uint16_t SP;

    // Utility methods
    uint8_t get_A();
    uint8_t get_F();
    uint8_t get_B();
    uint8_t get_C();
    uint8_t get_D();
    uint8_t get_E();
    uint8_t get_H();
    uint8_t get_L();

    void set_A(uint8_t);
    void set_F(uint8_t);
    void set_B(uint8_t);
    void set_C(uint8_t);
    void set_D(uint8_t);
    void set_E(uint8_t);
    void set_H(uint8_t);
    void set_L(uint8_t);

    void set_AF(uint16_t);
    void set_BC(uint16_t);
    void set_DE(uint16_t);
    void set_HL(uint16_t);
    void set_SP(uint16_t);

    uint16_t get_AF();
    uint16_t get_BC();
    uint16_t get_DE();
    uint16_t get_HL();
    uint16_t get_SP();

    void reset_z_flag();
    void reset_n_flag();
    void reset_h_flag();
    void reset_c_flag();

    bool get_z_flag();
    bool get_n_flag();
    bool get_h_flag();
    bool get_c_flag();

    void set_z_flag();
    void set_n_flag();
    void set_h_flag();
    void set_c_flag();

    void PUSH(uint16_t);
    uint16_t POP();

    int get_x(uint8_t);
    int get_y(uint8_t);
    int get_z(uint8_t);

    int get_p(uint8_t);
    int get_q(uint8_t);

    uint8_t get_reg_value(int);
    void set_reg_value(int, uint8_t);

    uint16_t get_ss_value(int);
    void set_ss_value(int, uint16_t);

    // Instructions

    // 8-bit loads
    void LD_r_r_(uint8_t);
    void LD_r_n(uint8_t);

    void ADD_s(uint8_t);
    void ADC_s(uint8_t);
    void SUB_s(uint8_t);
    void SBC_s(uint8_t);
    void AND_s(uint8_t);
    void OR_s(uint8_t);
    void XOR_s(uint8_t);
    void CP_s(uint8_t);
    void INC_r(uint8_t);
    void DEC_r(uint8_t);
    void INC_ss(uint8_t);
    void DEC_ss(uint8_t);

    // CB instructions
    void RLC_m(uint8_t);
    void RRC_m(uint8_t);
    void RL_m(uint8_t);
    void RR_m(uint8_t);
    void SWAP_m(uint8_t);
    void SLA_m(uint8_t);
    void SRA_m(uint8_t);
    void SRL_m(uint8_t);
    void BIT_b_m(uint8_t);
    void SET_b_m(uint8_t);
    void RES_b_m(uint8_t);

//    void LD_A__HL_();
    void LD_A__BC_();
    void LD_A__DE_();
    void LD_A__nn_();
    void LD__BC__A();
    void LD__DE__A();
    void LD__nn__A();
    void LD_A__C_();
    void LD__C__A();

    void LD_A__HLI_();
    void LD_A__HLD_();

    void LD__HLD__A();
    void LD__HLI__A();

    void LD__n__A();
    void LD_A__n_();

    // 16-bit loads
    void LD_BC_nn();
    void LD_DE_nn();
    void LD_HL_nn();
    void LD_SP_nn();
    void LD_SP_HL();
    void LDHL_SP_n();
    void LD__nn__SP();
    void PUSH_AF();
    void PUSH_BC();
    void PUSH_DE();
    void PUSH_HL();
    void POP_AF();
    void POP_BC();
    void POP_DE();
    void POP_HL();

    // 8-bit ALU
    void ADD_A_n();
    void ADC_A_n();
    void SUB_n();
    void SBC_n();
    void AND_n();
    void OR_n();
    void XOR_n();
    void CP_n();

    // 16-bit Arithmetic
    void ADD_HL_BC();
    void ADD_HL_DE();
    void ADD_HL_HL();
    void ADD_HL_SP();

    void ADD_SP_n();

    // Misc
    void DAA();
    void CPL();
    void CCF();
    void SCF();
    void NOP(uint8_t);
    void HALT();
    void STOP();
    void DI();
    void EI();

    // Rotates and Shifts
    void RLCA();
    void RLA();
    void RRCA();
    void RRA();

    // Jumps
    void JP_nn();
    void JP_NZ_nn();
    void JP_Z_nn();
    void JP_NC_nn();
    void JP_C_nn();
    void JP__HL_();
    void JR_n();
    void JR_NZ_nn();
    void JR_Z_nn();
    void JR_NC_nn();
    void JR_C_nn();

    // Calls
    void CALL_nn();
    void CALL_NZ_nn();
    void CALL_Z_nn();
    void CALL_NC_nn();
    void CALL_C_nn();

    // Restarts
    void RST(uint8_t);

    // Returns
    void RET();
    void RET_NZ();
    void RET_Z();
    void RET_NC();
    void RET_C();
    void RETI();

    MemoryBus * memory;
    Interrupt * interrupt;
    bool halt;

    explicit LR35902(MemoryBus *, Interrupt *);
    int execute_cycle();
    int cb_instruction();
    uint8_t fetch_byte();
    uint16_t fetch_word();

    bool test_bit(uint8_t, int);
    uint8_t set_bit(uint8_t, int);
    uint8_t reset_bit(uint8_t, int);
    bool get_carry(uint8_t, uint8_t);
    bool get_half_carry(uint8_t, uint8_t);
    bool get_borrow(uint8_t, uint8_t);
    bool get_half_borrow(uint8_t, uint8_t);

    int check_interrupts();
};

#endif //EMULATOR_Z80_H
