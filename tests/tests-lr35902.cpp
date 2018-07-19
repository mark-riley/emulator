#include "catch.hpp"

#include "LR35902.h"
#include "MemoryBus.h"

SCENARIO("LD A, B", "[8-bit transfer]") {
    GIVEN("Register A contains 32H and register B contains 45H") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 0x32 into register A
        cpu->set_A(0x32);

        // put 0x45 into register B
        cpu->set_B(0x45);

        // initialise the other registers
        cpu->set_C(0x00);
        cpu->set_D(0x00);
        cpu->set_E(0x00);
        cpu->set_H(0x00);
        cpu->set_L(0x00);

        cpu->set_SP(0x0000);
        cpu->PC = 0x0000;

        cpu->reset_c_flag();
        cpu->reset_h_flag();
        cpu->reset_n_flag();
        cpu->reset_z_flag();
        WHEN("the opcode LD A, B") {
            cpu->LD_r_r_(0x78);
            THEN("register A contains 45H and register B contains 45H") {
                REQUIRE(cpu->get_A() == 0x45);
                REQUIRE(cpu->get_B() == 0x45);

                // Check the other registers
                REQUIRE(cpu->get_C() == 0x00);
                REQUIRE(cpu->get_D() == 0x00);
                REQUIRE(cpu->get_E() == 0x00);
                REQUIRE(cpu->get_H() == 0x00);
                REQUIRE(cpu->get_L() == 0x00);

                REQUIRE(cpu->PC == 0x00);
                REQUIRE(cpu->get_SP() == 0x00);

                REQUIRE(cpu->get_c_flag() == 0);
                REQUIRE(cpu->get_h_flag() == 0);
                REQUIRE(cpu->get_n_flag() == 0);
                REQUIRE(cpu->get_z_flag() == 0);
            }
        }
    }
}

SCENARIO("LD B, D", "[8-bit transfer]") {
    GIVEN("Register B contains 32H and register D contains 45H") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 0x32 into register B
        cpu->set_B(0x32);

        // put 0x45 into register D
        cpu->set_D(0x45);

        // initialise the other registers
        cpu->set_A(0x00);
        cpu->set_C(0x00);
        cpu->set_E(0x00);
        cpu->set_H(0x00);
        cpu->set_L(0x00);

        cpu->set_SP(0x0000);
        cpu->PC = 0x0000;

        cpu->reset_c_flag();
        cpu->reset_h_flag();
        cpu->reset_n_flag();
        cpu->reset_z_flag();
        WHEN("the opcode LD B, D") {
            cpu->LD_r_r_(0x42);
            THEN("register B contains 45H and register D contains 45H") {
                REQUIRE(cpu->get_B() == 0x45);
                REQUIRE(cpu->get_D() == 0x45);

                // Check the other registers
                REQUIRE(cpu->get_A() == 0x00);
                REQUIRE(cpu->get_C() == 0x00);
                REQUIRE(cpu->get_E() == 0x00);
                REQUIRE(cpu->get_H() == 0x00);
                REQUIRE(cpu->get_L() == 0x00);

                REQUIRE(cpu->PC == 0x00);
                REQUIRE(cpu->get_SP() == 0x00);

                REQUIRE(cpu->get_c_flag() == 0);
                REQUIRE(cpu->get_h_flag() == 0);
                REQUIRE(cpu->get_n_flag() == 0);
                REQUIRE(cpu->get_z_flag() == 0);
            }
        }
    }
}

SCENARIO("LD B, nn", "[8-bit transfer]") {
    GIVEN("Register B contains 32H and the next byte is 24H") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 0x32 into register B
        cpu->set_B(0x32);

        // Intialise the other registers
        cpu->set_A(0x00);
        cpu->set_C(0x00);
        cpu->set_D(0x00);
        cpu->set_E(0x00);
        cpu->set_H(0x00);
        cpu->set_L(0x00);

        // put 0x45 into the first byte of RAM
        memory->write_byte(0xC000, 0x24);

        // reset the PC
        cpu->PC = 0xC000;
        WHEN("the opcode LD B, n") {
            cpu->LD_B_n();
            THEN("register B contains 24H") {
                REQUIRE(cpu->get_B() == 0x24);

                // Check the other registers
                REQUIRE(cpu->get_A() == 0x00);
                REQUIRE(cpu->get_C() == 0x00);
                REQUIRE(cpu->get_D() == 0x00);
                REQUIRE(cpu->get_E() == 0x00);
                REQUIRE(cpu->get_H() == 0x00);
                REQUIRE(cpu->get_L() == 0x00);
            }
        }
    }
}

SCENARIO("LD H, (HL)", "[8-bit transfer]") {
    GIVEN("Register pair HL contains C000H and RAM contains 5CH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000H into register HL
        cpu->set_HL(0xC000);

        // put 0x45 into the first byte of RAM
        memory->write_byte(0xC000, 0x5C);

        // reset the PC
        cpu->PC = 0xC000;
        WHEN("the opcode LD H, (HL)") {
            cpu->LD_r_r_(0x66);
            THEN("register H contains 5CH") {
                REQUIRE(cpu->get_H() == 0x5C);
            }
        }
    }
}

SCENARIO("LD (HL), A", "[8-bit transfer]") {
    GIVEN("Register pair HL contains 8AC5H and A contains 3CH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 8AC5H into register HL
        cpu->set_HL(0x8AC5);

        // put 0x45 into register A
        cpu->set_A(0x3C);

        WHEN("the opcode LD (HL), A") {
            cpu->LD_r_r_(0x77);
            THEN("memory address referenced by HL contains 3CH") {
                REQUIRE(memory->read_byte(cpu->get_HL()) == 0x3C);
                REQUIRE(memory->read_byte(0x8AC5) == 0x3C);
            }
        }
    }
}

SCENARIO("LD (HL), 0", "[8-bit transfer]") {
    GIVEN("Register pair HL contains 8AC5H and RAM contains 0") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 8AC5H into register HL
        cpu->set_HL(0x8AC5);

        // put 0x45 into the first byte of RAM
        memory->write_byte(0xC000, 0x00);

        // reset the PC
        cpu->PC = 0xC000;
        WHEN("the opcode LD (HL), n") {
            cpu->LD__HL__n();
            THEN("memory address referenced by HL contains 00H") {
                REQUIRE(memory->read_byte(cpu->get_HL()) == 0x00);
                REQUIRE(memory->read_byte(0x8AC5) == 0x00);
            }
        }
    }
}

SCENARIO("LD A, (BC)", "[8-bit transfer]") {
    GIVEN("Register pair BC contains C000H and RAM contains 2FH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000 into register BC
        cpu->set_BC(0xC000);

        // put 0x2F into the first byte of RAM
        memory->write_byte(0xC000, 0x2F);

        WHEN("the opcode LD A, (BC)") {
            cpu->LD_A__BC_();
            THEN("Register A contains 2FH") {
                REQUIRE(cpu->get_A() == 0x2F);
            }
        }
    }
}

SCENARIO("LD A, (DE)", "[8-bit transfer]") {
    GIVEN("Register pair DE contains C000H and RAM contains 5FH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000 into register BC
        cpu->set_DE(0xC000);

        // put 0x2F into the first byte of RAM
        memory->write_byte(0xC000, 0x5F);

        WHEN("the opcode LD A, (DE)") {
            cpu->LD_A__DE_();
            THEN("Register A contains 5FH") {
                REQUIRE(cpu->get_A() == 0x5F);
            }
        }
    }
}

SCENARIO("LD A, (C)", "[8-bit transfer]") {
    GIVEN("Register C contains 95H and RAM FF95H contains 2FH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000 into register BC
        cpu->set_C(0x95);

        // put 0x2F into the first byte of RAM
        memory->write_byte(0xFF95, 0x2F);

        WHEN("the opcode LD A, (BC)") {
            cpu->LD_A__C_();
            THEN("Register A contains 2FH") {
                REQUIRE(cpu->get_A() == 0x2F);
            }
        }
    }
}

SCENARIO("LD (C), A", "[8-bit transfer]") {
    GIVEN("Register C contains 9FH and register A contains 2FH") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put 9F into register C
        cpu->set_C(0x9F);

        // put 0x2F into register A
        cpu->set_A(0x2F);

        WHEN("the opcode LD (C), A") {
            cpu->LD__C__A();
            THEN("The memory location pointed at by register C contains 2Fh") {
                REQUIRE(memory->read_byte(0xFF00 | cpu->get_C()) == 0x2F);
                REQUIRE(memory->read_byte(0xFF9F) == 0x2F);

                REQUIRE(cpu->get_C() == 0x9F);
            }
        }
    }
}

SCENARIO("LD A, (n)", "[8-bit transfer]") {
    GIVEN("RAM contains FF43H") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000 into register BC
        cpu->set_A(0x00);

        // put 0x2F into the first byte of RAM
        memory->write_word(0xC000, 0xFF43);

        memory->write_byte(0xFF43, 0x2F);

        // reset the program counter
        cpu->PC = 0xC000;
        WHEN("the opcode LD A, (BC)") {
            cpu->LD_A__nn_();
            THEN("Register A contains 2FH") {
                REQUIRE(cpu->get_A() == 0x2F);
            }
        }
    }
}

SCENARIO("LD (n), A", "[8-bit transfer]") {
    GIVEN("RAM contains FF34H and register A contains 2Fh") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        // put C000 into register BC
        cpu->set_A(0x2F);

        // put 0x2F into the first byte of RAM
        memory->write_word(0xC000, 0xFF34);
        cpu->PC = 0xC000;
        WHEN("the opcode LD (nn), A") {
            cpu->LD__nn__A();
            THEN("Memory at FF34h contains 2Fh") {
                REQUIRE(memory->read_byte(0xFF34 == 0x2F));
                REQUIRE(memory->read_word(0xC000 == 0xFF34));
            }
        }
    }
}

SCENARIO("LD A, (nn)", "[8-bit transfer]") {
   GIVEN("RAM contains FF44h and FF44h contain 2Fh") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       // put 0x2F into the first byte of RAM
       memory->write_word(0xC000, 0xFF40);
       memory->write_byte(0xFF40, 0x2F);

       cpu->PC = 0xC000;
       WHEN("the opcode LD A, (nn)") {
           cpu->LD_A__nn_();
           THEN("Register A contains 2FH") {
               REQUIRE(cpu->get_A() == 0x2F);
           }
       }
   }
   GIVEN("RAM contains 8000h and 8000h contain 2Fh") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       // put 0x2F into the first byte of RAM
       memory->write_word(0xC000, 0x8000);
       memory->write_byte(0x8000, 0x2F);

       cpu->PC = 0xC000;
       WHEN("the opcode LD A, (nn)") {
           cpu->LD_A__nn_();
           THEN("Register A contains 2FH") {
               REQUIRE(cpu->get_A() == 0x2F);
           }
       }
   }
}

SCENARIO("LD (nn), A", "[8-bit transfer]") {
   GIVEN("RAM contains FF44h and register A contains 2Fh") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       // put 0x2F into the first byte of RAM
       memory->write_word(0xC000, 0xFF40);
       cpu->set_A(0x2F);

       cpu->PC = 0xC000;
       WHEN("the opcode LD (nn), A") {
           cpu->LD__nn__A();
           THEN("Register A contains 2FH") {
               REQUIRE(memory->read_byte(0xFF40) == 0x2F);
           }
       }
   }
   GIVEN("RAM contains 8000h and register A contains 2Fh") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       // put 0x2F into the first byte of RAM
       memory->write_word(0xC000, 0x8000);
       cpu->set_A(0x2F);

       cpu->PC = 0xC000;
       WHEN("the opcode LD (nn), A") {
           cpu->LD__nn__A();
           THEN("Register A contains 2FH") {
               REQUIRE(memory->read_byte(0x8000) == 0x2F);
           }
       }
   }
}

SCENARIO("LD A, (HLI)", "[8-bit transfer]") {
   GIVEN("Register pair HL contains C1FFh and RAM contains 56h") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_HL(0xC1FF);

       memory->write_byte(0xC1FF, 0x56);

       WHEN("the opcode LD A, (HLI)") {
           cpu->LD_A__HLI_();
           THEN("Register A contains 56h and HL contains C200h") {
               REQUIRE(cpu->get_A() == 0x56);
               REQUIRE(cpu->get_HL() == 0xC200);
           }
       }
   }
}

SCENARIO("LD A, (HLD)", "[8-bit transfer]") {
   GIVEN("Register pair HL contains 8A5Ch and RAM contains 3Ch") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_HL(0x8A5C);

       memory->write_byte(0x8A5C, 0x3C);

       WHEN("the opcode LD A, (HLD)") {
           cpu->LD_A__HLD_();
           THEN("Register A contains 3Ch and HL contains C200h") {
               REQUIRE(cpu->get_A() == 0x3C);
               REQUIRE(cpu->get_HL() == 0x8A5B);
           }
       }
   }
}

SCENARIO("LD (BC), A", "[8-bit transfer]") {
   GIVEN("Register pair BC contains C05Fh and register A contains 3Fh") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_BC(0xC05F);

       cpu->set_A(0x3F);

       WHEN("the opcode LD (BC), A") {
           cpu->LD__BC__A();
           THEN("memory pointed at by BC contains 3FH") {
               REQUIRE(memory->read_byte(cpu->get_BC()) == 0x3F);
               REQUIRE(memory->read_byte(0xC05F) == 0x3F);
           }
       }
   }
}

SCENARIO("LD (DE), A", "[8-bit transfer]") {
   GIVEN("Register pair DE contains C05Ch and register A contains 00h") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_DE(0xC05C);

       cpu->set_A(0x00);

       WHEN("the opcode LD (DE), A") {
           cpu->LD__DE__A();
           THEN("memory pointed at by DE contains 00H") {
               REQUIRE(memory->read_byte(cpu->get_DE()) == 0x00);
               REQUIRE(memory->read_byte(0xC05C) == 0x00);
           }
       }
   }
}

SCENARIO("LD (HLI), A", "[8-bit transfer]") {
   GIVEN("Register pair HL contains FFFFh and register A contains 56h") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_HL(0xFFFF);

       cpu->set_A(0x56);

       WHEN("the opcode LD (HLI), A") {
           cpu->LD__HLI__A();
           THEN("HL contains 0000h and memory address FFFFh contains 0x56") {
               REQUIRE(cpu->get_HL() == 0x0000);
               REQUIRE(memory->read_byte(0xFFFF) == 0x56);
           }
       }
   }
}

SCENARIO("LD (HLD), A", "[8-bit transfer]") {
   GIVEN("Register pair HL contains C000H and RAM contains 05H") {
       auto* memory = new MemoryBus;
       memory->init();

       auto* cpu = new LR35902;
       cpu->init(memory);

       cpu->set_HL(0xC000);

       cpu->set_A(0x05);

       WHEN("the opcode LD (HLD), A") {
           cpu->LD__HLD__A();
           THEN("HL contains 0000h and memory address FFFFh contains 0x56") {
               REQUIRE(cpu->get_HL() == 0xBFFF);
               REQUIRE(memory->read_byte(0xC000) == 0x05);
           }
       }
   }
}


// 16 bit transfers
//LD HL, 3A5Bh ; H ← 3Ah, L ← 5Bh

SCENARIO("PUSH qq", "[16-Bit Transfer]") {
    GIVEN("SP = FFFEh and B = 3Ch and C=5Fh") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        cpu->set_SP(0xFFFE);
        cpu->set_B(0x3C);
        cpu->set_C(0x5F);

        WHEN("we call PUSH BC") {
            cpu->PUSH_BC();

            THEN("SP = FFFCh and memory FFFDh = 3Ch and FFFCh = 5Fh") {
                REQUIRE(cpu->get_SP() == 0xFFFC);
                REQUIRE(memory->read_byte(0xFFFD) == 0x3C);
                REQUIRE(memory->read_byte(0xFFFC) == 0x5F);
            }
        }
    }
}

SCENARIO("POP qq", "[16-Bit Transfer]") {
    GIVEN("SP = FFFCh and (FFFCh) = 5Fh and (FFFDh)=3Ch") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);

        cpu->set_SP(0xFFFC);
        memory->write_byte(0xFFFC, 0x5F);
        memory->write_byte(0xFFFD, 0x3C);

        WHEN("we call POP BC") {
            cpu->POP_BC();

            THEN("SP = FFFEh and B 3Ch and C = 5Fh") {
                REQUIRE(cpu->get_SP() == 0xFFFE);
                REQUIRE(cpu->get_B() == 0x3C);
                REQUIRE(cpu->get_C() == 0x5F);
            }
        }
    }
}

//When SP = 0xFFF8,
//        LDHL SP, 2 ; HL ← 0xFFFA, CY ← 0, H ← 0, N ← 0, Z ← 0

//When SP = FFF8h,
//        LD (C100h) , SP ; C100h ← F8h
//        C101h← FFh

// 8 bit arithmetic
//When A = 0x3A and B = 0xC6,
//        ADD A, B ; A ← 0, Z ← 1, H ← 1, N ← 0, CY ← 1

//When A = 3Ch,
//        ADD A. FFh ; A ← 3Bh, Z ← 0, H ← 1, N ← 0, CY ← 1

//When A = 3Ch and (HL) = 12h,
//        ADD A, (HL) ; A ← 4Eh, Z ← 0, H ← 0, N ← 0, CY ← 0

//When A = E1h, E = 0Fh, (HL) = 1Eh, and CY = 1,
//        ADC A, E ; A ← F1h, Z ← 0, H ← 1, CY ← 0
//ADC A, 3Bh ; A ← 1Dh, Z ← 0, H ← 0, CY ← -1
//ADC A, (HL) ; A ← 00h, Z ← 1, H ← 1, CY ← 1

//When A = 3Eh, E = 3Eh, and (HL) = 40h,
//        SUB E ; A ← 00h, Z ← 1, H ← 0, N ← 1 CY ← 0
//SUB 0Fh ; A ← 2Fh, Z ← 0, H ← 1, N ← 1 CY← 0
//SUB (HL) ; A ← FEh, Z ← 0, H ← 0, N ← 1 CY ← 1

//When A = 3Bh, (HL) = 4Fh, H = 2Ah, and CY = 1,
//        SBC A, H ; A ← 10h, Z ← 0, H ← 0, N ← 1 CY ← 0
//SBC A, 3Ah ; A ← 00h, Z ← 1, H ← 0, N ← 1 CY ← 0
//SBC A, (HL) ; A ← EBh, Z ← 0, H ← 1, N ← 1 CY ← 1

//When A = 5Ah, L = 3Fh and (HL) = 0h,
//        AND L ; A ← 1Ah, Z ← 0, H ← 1, N ← 0 CY ← 0
//AND 38h ; A ← 18h, Z ← 0, H ← 1, N ← 0 CY ← 0
//AND (HL) ; A ← 00h, Z ← 1, H ← 1, N ← 0 CY ← 0

//When A = 5Ah, (HL) = 0Fh,
//        OR A ; A ← 5Ah, Z ← 0
//OR 3 ; A ← 5Bh, Z ← 0
//OR (HL) ; A ← 5Fh, Z ← 0

//When A = FFh and (HL) = 8Ah,
//        XOR A ; A ← 00h, Z ← 1
//XOR 0x0F ; A ← F0h, Z ← 0
//XOR (HL) ; A ← 75h, Z ← 0

//When A = 3Ch, B = 2Fh, and (HL) = 40h,
//        CP B ; Z ← 0, H ← 1, N ← 1, CY ← 0
//CP 3Ch ; Z ← 1, H ← 0, N ← 1, CY ← 0
//CP (HL) ; Z ← 0, H ← 0, N ← 1, CY ← 1

//When A = FFh,
//        INC A ; A ← 0, Z ← 1, H ← 1, N ← 0

//When (HL) = 0x50,
//        INC (HL) ; (HL) ← 0x51, Z ← 0, H ← 0, N ← 0

//When L = 01h,
//        DEC L ; L ← 0, Z ← 1, H ← 0, N ← 1

//When (HL) = 00h,
//        DEC (HL) ; (HL) ← FFh, Z ← 0, H ← 1, N ← 1

// 16 bit arithmetic
//When HL = 8A23h, BC = 0605h,
//        ADD HL, BC ; HL ← 9028h, H ← 1, N ← 0, CY ← 0
//ADD HL, HL ; HL ← 1446h, H ← 1, N ← 0, CY ← 1

//SP = FFF8h
//ADD SP, 2 ; SP ← 0xFFFA, CY ← 0, H ← 0, N ← 0, Z ← 0

//When DE = 235Fh,
//        INC DE ; DE ← 2360h

//When DE = 235Fh,
//        DEC DE ; DE ← 235Eh

SCENARIO("DAA ADD", "[General Purpose]") {
    GIVEN("A = 45h and B = 38h") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);
        cpu->set_A(0x45);
        cpu->set_B(0x38);

        WHEN("we call ADD A, B followed by DAA") {
            cpu->ADD_s(0x80);
            cpu->DAA();

            THEN("A = 83h and n flag is unset and c flag is unset") {
                REQUIRE(!cpu->get_n_flag());
                REQUIRE(!cpu->get_c_flag());
                REQUIRE(cpu->get_A() == 0x83);
            }
        }
    }
}
SCENARIO("DAA SUB", "[General Purpose]") {
    GIVEN("A = 45h and B = 38h") {
        auto* memory = new MemoryBus;
        memory->init();

        auto* cpu = new LR35902;
        cpu->init(memory);
        cpu->set_A(0x83);
        cpu->set_B(0x38);

        WHEN("we call SUB A, B followed by DAA") {
            cpu->SUB_s(0x90);
            cpu->DAA();

            THEN("A = 45h and n flag is set") {
                REQUIRE(cpu->get_n_flag());
                REQUIRE(cpu->get_A() == 0x45);
            }
        }
    }
}