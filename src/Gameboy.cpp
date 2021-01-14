#include "Gameboy.h"

Gameboy::Gameboy(Cartridge* c, std::vector<uint8_t> fileBuf) {
    cartridge = c;
    interrupt = new Interrupt();
    timer = new Timer(interrupt);
    mmu = new MemoryBus(cartridge, timer, interrupt);
    mmu->load_boot_rom(fileBuf);
    cpu = new LR35902(mmu, interrupt, false);
    ppu = new Display(mmu, interrupt);
    render = new Render(ppu);
}

void Gameboy::powerOn() {
    int running = true;
    SDL_Event e;
    render->init(cartridge->getTitle());

//    4194304  // clock cycles per second (2^22)
//    59.73 hz - refresh rate
//    4194304 / 59.73 = 70221
    const int MAX_CYCLES = 70368;
//5016
    while (running) {
        int cyclesThisUpdate = 0;
        while(cyclesThisUpdate <= MAX_CYCLES) {
            // start
            int cycles = cpu->check_interrupts();
            cycles += cpu->execute_cycle();
            cyclesThisUpdate += cycles;
            ppu->UpdateGraphics(cycles);
            timer->doTimers(cycles);
        }

        render->renderScreen();

        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                uint8_t input = 0x00;
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_UP:
                        input |= 1 << 4;
                        input |= 1 << 2;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_DOWN:
                        input |= 1 << 4;
                        input |= 1 << 3;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_LEFT:
                        input |= 1 << 4;
                        input |= 1 << 1;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_RIGHT:
                        input |= 1 << 4;
                        input |= 1;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_z:
                        input |= 1 << 5;
                        input |= 1 << 1;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_x:
                        input |= 1 << 5;
                        input |= 1;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_RETURN:
                        input |= 1 << 5;
                        input |= 1 << 3;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    case SDLK_RSHIFT:
                        input |= 1 << 5;
                        input |= 1 << 2;
                        interrupt->request_interrupt(interrupt->CONTROLLER);
                        break;
                    default:
                        break;
                }
                mmu->write_byte(0xFF00, input);
            }
        }
    }
}