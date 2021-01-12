#include <SDL_log.h>
#include "LR35902.h"

LR35902::LR35902(MemoryBus * memory_bus, Interrupt * i, bool skip_bios) {
//    memory_bus->write_byte(0xFF00, 0xCF);
//    memory_bus->write_byte(0xFF05, 0x0);
//    memory_bus->write_byte(0xFF06, 0x0);
//    memory_bus->write_byte(0xFF07, 0x0);
//    memory_bus->write_byte(0xFF10, 0x80);
//    memory_bus->write_byte(0xFF11, 0xBF);
//    memory_bus->write_byte(0xFF12, 0xF3);
//    memory_bus->write_byte(0xFF14, 0xBF);
//    memory_bus->write_byte(0xFF16, 0x3F);
//    memory_bus->write_byte(0xFF17, 0x00);
//    memory_bus->write_byte(0xFF19, 0xBF);
//    memory_bus->write_byte(0xFF1A, 0x7F);
//    memory_bus->write_byte(0xFF1B, 0xFF);
//    memory_bus->write_byte(0xFF1C, 0x9F);
//    memory_bus->write_byte(0xFF1E, 0xBF);
//    memory_bus->write_byte(0xFF20, 0xFF);
//    memory_bus->write_byte(0xFF21, 0x00);
//    memory_bus->write_byte(0xFF22, 0x00);
//    memory_bus->write_byte(0xFF23, 0xBF);
//    memory_bus->write_byte(0xFF24, 0x77);
//    memory_bus->write_byte(0xFF25, 0xF3);
//    memory_bus->write_byte(0xFF26, 0xF1);

//    memory_bus->write_byte(0xFF40, 0x91);
//    memory_bus->write_byte(0xFF42, 0x00);
    memory_bus->write_byte(0xFF43, 0x00);
//    memory_bus->write_byte(0xFF44, 0x00);
//    memory_bus->write_byte(0xFF45, 0x00);
//    memory_bus->write_byte(0xFF47, 0xFC);
//    memory_bus->write_byte(0xFF48, 0xFF);
//    memory_bus->write_byte(0xFF49, 0xFF);
//    memory_bus->write_byte(0xFF4A, 0x00);
//    memory_bus->write_byte(0xFF4B, 0x00);

    //    memory_bus->write_byte(0xFF0F, 0x00);
//    memory_bus->write_byte(0xFFFF, 0x00);

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
    int cycles = 0;
    if (halt) {
        cycles += 4;
        return cycles;
    }
    uint8_t current_opcode = fetch_byte();
//    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "opcode: %02X AF: %04X BC: %04X DE: %04X HL: %04X SP: %04X PC: %04X\n", current_opcode, get_AF(), get_BC(), get_DE(), get_HL(), SP, PC - 1);
    uint16_t old_pc = PC;

    switch (current_opcode) {
        // 8-bit loads
        case 0x7F: // ld a, a
        case 0x78: // ld a, b
        case 0x79: // ld a, c
        case 0x7A: // ld a, d
        case 0x7B: // ld a, e
        case 0x7C: // ld a, h
        case 0x7D: // ld a, l

        case 0x47: // ld b, a
        case 0x40: // ld b, b
        case 0x41: // ld b, c
        case 0x42: // ld b, d
        case 0x43: // ld b, e
        case 0x44: // ld b, h
        case 0x45: // ld b, l

        case 0x4F: // ld c, a
        case 0x48: // ld c, b
        case 0x49: // ld c, c
        case 0x4A: // ld c, d
        case 0x4B: // ld c, e
        case 0x4C: // ld c, h
        case 0x4D: // ld c, l

        case 0x57: // ld d, a
        case 0x50: // ld d, b
        case 0x51: // ld d, c
        case 0x52: // ld d, d
        case 0x53: // ld d, e
        case 0x54: // ld d, h
        case 0x55: // ld d, l

        case 0x5F: // ld e, a
        case 0x58: // ld e, b
        case 0x59: // ld e, c
        case 0x5A: // ld e, d
        case 0x5B: // ld e, e
        case 0x5C: // ld e, h
        case 0x5D: // ld e, l

        case 0x67: // ld h, a
        case 0x60: // ld h, b
        case 0x61: // ld h, c
        case 0x62: // ld h, d
        case 0x63: // ld h, e
        case 0x64: // ld h, h
        case 0x65: // ld h, l

        case 0x6F: // ld l, a
        case 0x68: // ld l, b
        case 0x69: // ld l, c
        case 0x6A: // ld l, d
        case 0x6B: // ld l, e
        case 0x6C: // ld l, h
        case 0x6D: // ld l, l
            cycles = 4;
            LD_r_r_(current_opcode);
            break;

        case 0x7E: // ld a, (hl)
        case 0x46: // ld b, (hl)
        case 0x4E: // ld c, (hl)
        case 0x56: // ld d, (hl)
        case 0x5E: // ld e, (hl)
        case 0x66: // ld h, (hl)
        case 0x6E: // ld l, (hl)

        case 0x77: // ld (hl), a
        case 0x70: // ld (hl), b
        case 0x71: // ld (hl), c
        case 0x72: // ld (hl), d
        case 0x73: // ld (hl), e
        case 0x74: // ld (hl), h
        case 0x75: // ld (hl), l
            // different block due to different cycles
            cycles = 8;
            LD_r_r_(current_opcode);
            break;

        case 0x3E: // ld a, n
        case 0x06: // ld b, n
        case 0x0E: // ld c, n
        case 0x16: // ld d, n
        case 0x1E: // ld e, n
        case 0x26: // ld h, n
        case 0x2E: // ld l, n
            cycles = 8;
            LD_r_n(current_opcode);
            break;

        case 0x36: // ld (hl), n
            cycles = 12;
            LD_r_n(current_opcode);
            break;

        case 0x0A: // ld a, (BC)
            cycles = 8;
            LD_A__BC_();
            break;

        case 0x1A: // ld a, (DE)
            cycles = 8;
            LD_A__DE_();
            break;

        case 0xF2: // ld a, (C)
            cycles = 8;
            LD_A__C_();
            break;

        case 0xE2: // ld (C), a
            cycles = 8;
            LD__C__A();
            break;

        case 0xF0: // ld a, (n)
            cycles = 12;
            LD_A__n_();
            break;

        case 0xE0: // ld (n), a
            cycles = 12;
            LD__n__A();
            break;

        case 0xFA: // ld a, (nn)
            cycles = 16;
            LD_A__nn_();
            break;

        case 0xEA: // ld (nn), a
            cycles = 16;
            LD__nn__A();
            break;

        case 0x2A: // ld a, (hli)
            cycles = 8;
            LD_A__HLI_();
            break;
        case 0x3A: // ld a, (hld)
            cycles = 8;
            LD_A__HLD_();
            break;
        case 0x02: // ld (bc), a
            cycles = 8;
            LD__BC__A();
            break;
        case 0x12: // ld (de), a
            cycles = 8;
            LD__DE__A();
            break;
        case 0x22: // ld (hli), a
            cycles = 8;
            LD__HLI__A();
            break;
        case 0x32: // ld (hld), a
            cycles = 8;
            LD__HLD__A();
            break;

        // 16-bit loads
        case 0x01: // ld bc, nn
            cycles = 12;
            LD_BC_nn();
            break;
        case 0x11: // ld de, nn
            cycles = 12;
            LD_DE_nn();
            break;
        case 0x21: // ld hl, nn
            cycles = 12;
            LD_HL_nn();
            break;
        case 0x31: // ld sp, nn
            cycles = 12;
            LD_SP_nn();
            break;

        case 0xF9: // ld sp, hl
            cycles = 8;
            LD_SP_HL();
            break;

        case 0xC5: // push BC
            cycles = 16;
            PUSH_BC();
            break;
        case 0xD5: // push DE
            cycles = 16;
            PUSH_DE();
            break;
        case 0xE5: // push HL
            cycles = 16;
            PUSH_HL();
            break;
        case 0xF5: // push AF
            cycles = 16;
            PUSH_AF();
            break;

        case 0xC1: // pop BC
            cycles = 12;
            POP_BC();
            break;
        case 0xD1: // pop DE
            cycles = 12;
            POP_DE();
            break;
        case 0xE1: // pop HL
            cycles = 12;
            POP_HL();
            break;
        case 0xF1: // pop AF
            cycles = 12;
            POP_AF();
            break;

        case 0xF8: // ldhl sp, e
            cycles = 12;
            LDHL_SP_n();
            break;

        case 0x08: // ldhl sp, e
            cycles = 20;
            LD__nn__SP();
            break;

        // 8-bit arithmetic and logical operation instructions
        case 0x87: // add a, a
        case 0x80: // add a, b
        case 0x81: // add a, c
        case 0x82: // add a, d
        case 0x83: // add a, e
        case 0x84: // add a, h
        case 0x85: // add a, l
            cycles = 4;
            ADD_s(current_opcode);
            break;

        case 0xC6: // add a, n
            cycles = 8;
            ADD_A_n();
            break;

        case 0x86: // add a, (hl)
            cycles = 8;
            ADD_s(current_opcode);
            break;

        case 0x8F: // adc a, a
        case 0x88: // adc a, b
        case 0x89: // adc a, c
        case 0x8A: // adc a, d
        case 0x8B: // adc a, e
        case 0x8C: // adc a, h
        case 0x8D: // adc a, l
            cycles = 4;
            ADC_s(current_opcode);
            break;

        case 0xCE: // adc a, n
            cycles = 8;
            ADC_A_n();
            break;

        case 0x8E: // adc a, (hl)
            cycles = 8;
            ADC_s(current_opcode);
            break;

        case 0x97: // sub a, a
        case 0x90: // sub a, b
        case 0x91: // sub a, c
        case 0x92: // sub a, d
        case 0x93: // sub a, e
        case 0x94: // sub a, h
        case 0x95: // sub a, l
            cycles = 4;
            SUB_s(current_opcode);
            break;

        case 0xD6: // sub a, n
            cycles = 8;
            SUB_n();
            break;

        case 0x96: // sub a, (hl)
            cycles = 8;
            SUB_s(current_opcode);
            break;

        case 0x9F: // sbc a, a
        case 0x98: // sbc a, b
        case 0x99: // sbc a, c
        case 0x9A: // sbc a, d
        case 0x9B: // sbc a, e
        case 0x9C: // sbc a, h
        case 0x9D: // sbc a, l
            cycles = 4;
            SBC_s(current_opcode);
            break;

        case 0xDE: // sbc a, n
            cycles = 8;
            SBC_n();
            break;

        case 0x9E: // sbc a, (hl)
            cycles = 8;
            SBC_s(current_opcode);
            break;

        case 0xA7: // and a, a
        case 0xA0: // and a, b
        case 0xA1: // and a, c
        case 0xA2: // and a, d
        case 0xA3: // and a, e
        case 0xA4: // and a, h
        case 0xA5: // and a, l
            cycles = 4;
            AND_s(current_opcode);
            break;

        case 0xE6: // and a, n
            cycles = 8;
            AND_n();
            break;

        case 0xA6: // and a, (hl)
            cycles = 8;
            AND_s(current_opcode);
            break;

        case 0xB7: // or a, a
        case 0xB0: // or a, b
        case 0xB1: // or a, c
        case 0xB2: // or a, d
        case 0xB3: // or a, e
        case 0xB4: // or a, h
        case 0xB5: // or a, l
            cycles = 4;
            OR_s(current_opcode);
            break;

        case 0xF6: // or a, n
            cycles = 8;
            OR_n();
            break;

        case 0xB6: // or a, (hl)
            cycles = 8;
            OR_s(current_opcode);
            break;

        case 0xAF: // xor a, a
        case 0xA8: // xor a, b
        case 0xA9: // xor a, c
        case 0xAA: // xor a, d
        case 0xAB: // xor a, e
        case 0xAC: // xor a, h
        case 0xAD: // xor a, l
            cycles = 4;
            XOR_s(current_opcode);
            break;

        case 0xEE: // xor a, n
            cycles = 8;
            XOR_n();
            break;

        case 0xAE: // xor a, (hl)
            cycles = 8;
            XOR_s(current_opcode);
            break;

        case 0xBF: // cp a, a
        case 0xB8: // cp a, b
        case 0xB9: // cp a, c
        case 0xBA: // cp a, d
        case 0xBB: // cp a, e
        case 0xBC: // cp a, h
        case 0xBD: // cp a, l
            cycles = 4;
            CP_s(current_opcode);
            break;

        case 0xFE: // cp a, n
            cycles = 8;
            CP_n();
            break;

        case 0xBE: // cp a, (hl)
            cycles = 8;
            CP_s(current_opcode);
            break;

        case 0x3C: // inc a
        case 0x04: // inc b
        case 0x0C: // inc c
        case 0x14: // inc d
        case 0x1C: // inc e
        case 0x24: // inc h
        case 0x2C: // inc l
            cycles = 4;
            INC_r(current_opcode);
            break;

        case 0x34: // inc (hl)
            cycles = 12;
            INC_r(current_opcode);
            break;

        case 0x3D: // dec a
        case 0x05: // dec b
        case 0x0D: // dec c
        case 0x15: // dec d
        case 0x1D: // dec e
        case 0x25: // dec h
        case 0x2D: // dec l
            cycles = 4;
            DEC_r(current_opcode);
            break;

        case 0x35: // dec (hl)
            cycles = 12;
            DEC_r(current_opcode);
            break;

        // 16-bit Arithmetic Operation Instructions
        case 0x09: // add hl, bc
            cycles = 8;
            ADD_HL_BC();
            break;

        case 0x19: // add hl, de
            cycles = 8;
            ADD_HL_DE();
            break;

        case 0x29: // add hl, hl
            cycles = 8;
            ADD_HL_HL();
            break;

        case 0x39: // add hl, sp
            cycles = 8;
            ADD_HL_SP();
            break;

        case 0xE8: // add sp, e
            cycles = 16;
            ADD_SP_n();
            break;

        case 0x03: // inc bc
        case 0x13: // inc de
        case 0x23: // inc hl
        case 0x33: // inc sp
            cycles = 8;
            INC_ss(current_opcode);
            break;

        case 0x0B: // inc bc
        case 0x1B: // inc de
        case 0x2B: // inc hl
        case 0x3B: // inc sp
            cycles = 8;
            DEC_ss(current_opcode);
            break;

        // Rotate Shift Instructions
        case 0x07: // rlca
            cycles = 4;
            RLCA();
            break;

        case 0x17: // rla
            cycles = 4;
            RLA();
            break;

        case 0x0F: // rrca
            cycles = 4;
            RRCA();
            break;

        case 0x1F: // rra
            cycles = 4;
            RRA();
            break;

        case 0x76: // halt
            cycles = 4;
            HALT();
            break;



        case 0x00:
            cycles += 4;
            NOP(current_opcode);
            break;

        case 0x10:
            cycles += 4;
            STOP();
            break;
        case 0x18:
            cycles += 12;
            JR_n();
            break;

        case 0x20:
            cycles += 8;
            JR_NZ_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0x27:
            cycles += 4;
            DAA();
            break;
        case 0x28:
            cycles += 8;
            JR_Z_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0x2F:
            cycles += 4;
            CPL();
            break;

        case 0x30:
            cycles += 8;
            JR_NC_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0x37:
            cycles += 4;
            SCF();
            break;
        case 0x38:
            cycles += 8;
            JR_C_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0x3F:
            cycles += 4;
            CCF();
            break;

        case 0xC0:
            cycles += 8;
            RET_NZ();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xC2:
            cycles += 12;
            JP_NZ_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0xC3:
            cycles += 16;
            JP_nn();
            break;
        case 0xC4:
            cycles += 12;
            CALL_NZ_nn();
            if (PC != old_pc) {
                cycles += 8;
            }
            break;
        case 0xC7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xC8:
            cycles += 8;
            RET_Z();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xC9:
            cycles += 16;
            RET();
            break;
        case 0xCA:
            cycles += 12;
            JP_Z_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0xCB:
            cycles = cb_instruction();
            break;
        case 0xCC:
            cycles += 12;
            CALL_Z_nn();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xCD:
            cycles += 24;
            CALL_nn();
            break;
        case 0xCF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xD0:
            cycles += 8;
            RET_NC();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xD2:
            cycles += 12;
            JP_NC_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0xD3:
            cycles += 4;
            break;
        case 0xD4:
            cycles += 12;
            CALL_NC_nn();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xD7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xD8:
            cycles += 8;
            RET_C();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xD9:
            cycles += 16;
            RETI();
            break;
        case 0xDA:
            cycles += 16;
            JP_C_nn();
            if (PC != old_pc) {
                cycles += 4;
            }
            break;
        case 0xDB:
            cycles += 4;
            break;
        case 0xDC:
            cycles += 12;
            CALL_C_nn();
            if (PC != old_pc) {
                cycles += 12;
            }
            break;
        case 0xDD:
            cycles += 4;
            break;
        case 0xDF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xE3:
            cycles += 4;
            break;
        case 0xE4:
            cycles += 4;
            break;
        case 0xE7:
            cycles += 16;
            RST(current_opcode);
            break;
        case 0xE9:
            cycles += 4;
            JP__HL_();
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
        case 0xEF:
            cycles += 16;
            RST(current_opcode);
            break;

        case 0xF3:
            cycles += 4;
            DI();
            break;
        case 0xF4:
            cycles += 4;
            break;
        case 0xF7:
            cycles += 16;
            RST(current_opcode);
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
        case 0x00:
        case 0x01:
        case 0x02:
        case 0x03:
        case 0x04:
        case 0x05:
        case 0x06:
        case 0x07:
            cycles += 8;
            RLC_m(current_opcode);
            break;
        case 0x08:
        case 0x09:
        case 0x0A:
        case 0x0B:
        case 0x0C:
        case 0x0D:
        case 0x0E:
        case 0x0F:
            cycles += 8;
            RRC_m(current_opcode);
            break;
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
        case 0x17:
            cycles += 8;
            RL_m(current_opcode);
            break;
        case 0x18:
        case 0x19:
        case 0x1A:
        case 0x1B:
        case 0x1C:
        case 0x1D:
        case 0x1E:
        case 0x1F:
            cycles += 8;
            RR_m(current_opcode);
            break;
        case 0x20:
        case 0x21:
        case 0x22:
        case 0x23:
        case 0x24:
        case 0x25:
        case 0x26:
        case 0x27:
            cycles += 8;
            SLA_m(current_opcode);
            break;
        case 0x28:
        case 0x29:
        case 0x2A:
        case 0x2B:
        case 0x2C:
        case 0x2D:
        case 0x2E:
        case 0x2F:
            cycles += 8;
            SRA_m(current_opcode);
            break;
        case 0x30:
        case 0x31:
        case 0x32:
        case 0x33:
        case 0x34:
        case 0x35:
        case 0x36:
        case 0x37:
            cycles += 8;
            SWAP_m(current_opcode);
            break;
        case 0x38:
        case 0x39:
        case 0x3A:
        case 0x3B:
        case 0x3C:
        case 0x3D:
        case 0x3E:
        case 0x3F:
            cycles += 8;
            SRL_m(current_opcode);
            break;
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
        case 0x4C:
        case 0x4D:
        case 0x4E:
        case 0x4F:
        case 0x50:
        case 0x51:
        case 0x52:
        case 0x53:
        case 0x54:
        case 0x55:
        case 0x56:
        case 0x57:
        case 0x58:
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x60:
        case 0x61:
        case 0x62:
        case 0x63:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x67:
        case 0x68:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6C:
        case 0x6D:
        case 0x6E:
        case 0x6F:
        case 0x70:
        case 0x71:
        case 0x72:
        case 0x73:
        case 0x74:
        case 0x75:
        case 0x76:
        case 0x77:
        case 0x78:
        case 0x79:
        case 0x7A:
        case 0x7B:
        case 0x7C:
        case 0x7D:
        case 0x7E:
        case 0x7F:
            cycles += 8;
            BIT_b_m(current_opcode);
            break;
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x90:
        case 0x91:
        case 0x92:
        case 0x93:
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        case 0x99:
        case 0x9A:
        case 0x9B:
        case 0x9C:
        case 0x9D:
        case 0x9E:
        case 0x9F:
        case 0xA0:
        case 0xA1:
        case 0xA2:
        case 0xA3:
        case 0xA4:
        case 0xA5:
        case 0xA6:
        case 0xA7:
        case 0xA8:
        case 0xA9:
        case 0xAA:
        case 0xAB:
        case 0xAC:
        case 0xAD:
        case 0xAE:
        case 0xAF:
        case 0xB0:
        case 0xB1:
        case 0xB2:
        case 0xB3:
        case 0xB4:
        case 0xB5:
        case 0xB6:
        case 0xB7:
        case 0xB8:
        case 0xB9:
        case 0xBA:
        case 0xBB:
        case 0xBC:
        case 0xBD:
        case 0xBE:
        case 0xBF:
            cycles += 8;
            RES_b_m(current_opcode);
            break;
        case 0xC0:
        case 0xC1:
        case 0xC2:
        case 0xC3:
        case 0xC4:
        case 0xC5:
        case 0xC6:
        case 0xC7:
        case 0xC8:
        case 0xC9:
        case 0xCA:
        case 0xCB:
        case 0xCC:
        case 0xCD:
        case 0xCE:
        case 0xCF:
        case 0xD0:
        case 0xD1:
        case 0xD2:
        case 0xD3:
        case 0xD4:
        case 0xD5:
        case 0xD6:
        case 0xD7:
        case 0xD8:
        case 0xD9:
        case 0xDA:
        case 0xDB:
        case 0xDC:
        case 0xDD:
        case 0xDE:
        case 0xDF:
        case 0xE0:
        case 0xE1:
        case 0xE2:
        case 0xE3:
        case 0xE4:
        case 0xE5:
        case 0xE6:
        case 0xE7:
        case 0xE8:
        case 0xE9:
        case 0xEA:
        case 0xEB:
        case 0xEC:
        case 0xED:
        case 0xEE:
        case 0xEF:
        case 0xF0:
        case 0xF1:
        case 0xF2:
        case 0xF3:
        case 0xF4:
        case 0xF5:
        case 0xF6:
        case 0xF7:
        case 0xF8:
        case 0xF9:
        case 0xFA:
        case 0xFB:
        case 0xFC:
        case 0xFD:
        case 0xFE:
        case 0xFF:
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
    memory->write_byte(get_SP() - 2, data & 0x00FF);
    set_SP(get_SP() - 2);
}

uint16_t LR35902::POP() {
    uint16_t data = (memory->read_byte(get_SP() + 1) << 8) | memory->read_byte(get_SP());
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
