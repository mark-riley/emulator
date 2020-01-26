#include "LR35902.h"
#include "Display.h"
#include "Cartridge.h"
#include "Render.h"
#include "Timer.h"
#include "Interrupt.h"
#include "file-utils.h"

#define BOOT_ROM "dmg_boot.bin"
//#define GAME_ROM "Dr. Mario (World) (Rev A).gb"
//#define GAME_ROM "Pokemon - Blue Version (USA, Europe) (SGB Enhanced).gb"
//#define GAME_ROM "Legend of Zelda, The - Link's Awakening (U) (V1.2) [!].gb"
//#define GAME_ROM "Legend of Zelda, The - Link's Awakening DX (U) (V1.2) [C][!].gbc"
//#define GAME_ROM "Tetris (World).gb"
//#define GAME_ROM "Alleyway (World).gb"
//#define GAME_ROM "Super Mario Land (World) (Rev A).gb"
//#define GAME_ROM "dmg_test_prog_ver1.gb"
//#define GAME_ROM "sprite_priority.gb"
//#define GAME_ROM "dycptest2.gb"
//#define GAME_ROM "add_sp_e_timing.gb"
#define GAME_ROM "instr_timing.gb"
//#define GAME_ROM "interrupt_time.gb"
//#define GAME_ROM "01-read_timing.gb"
//#define GAME_ROM "02-write_timing.gb"
//#define GAME_ROM "03-modify_timing.gb"
//#define GAME_ROM "cpu_instrs.gb"
//#define GAME_ROM "01-special.gb"
//#define GAME_ROM "02-interrupts.gb"
//#define GAME_ROM "03-op sp,hl.gb"
//#define GAME_ROM "04-op r,imm.gb"
//#define GAME_ROM "05-op rp.gb"
//#define GAME_ROM "06-ld r,r.gb"
//#define GAME_ROM "07-jr,jp,call,ret,rst.gb"
//#define GAME_ROM "08-misc instrs.gb"
//#define GAME_ROM "09-op r,r.gb"
//#define GAME_ROM "10-bit ops.gb"
//#define GAME_ROM "11-op a,(hl).gb"
//#define GAME_ROM "bgbtest.gb"
//#define GAME_ROM "opus5.gb"
//#define GAME_ROM "lyc.gb"
//#define GAME_ROM "naughtyemu.gb"
//#define GAME_ROM "ie_push.gb"

int main (int argv, char** args) {
    int running = true;
    SDL_Event e;

    // Loading the boot rom
    const char * filePath = BOOT_ROM;
    std::vector<uint8_t> fileBuf = readFileToBuffer(filePath);

    const char * rompath = GAME_ROM;
    std::vector<uint8_t> romBuf = readFileToBuffer(rompath);

    auto cartridge = new Cartridge(romBuf);
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cartridge Title: %s\n", cartridge->getTitle().c_str());
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "Cartridge Type: %X\n", cartridge->getCartridgeType());
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "ROM Size: %d\n", cartridge->getRomBanks());
    SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "RAM Size: %d\n", cartridge->getRamBanks());

    auto * interrupt = new Interrupt();

    auto * timer = new Timer(interrupt);

    auto *memory_bus = new MemoryBus(cartridge, timer, interrupt);
    memory_bus->load_boot_rom(fileBuf);

    auto *cpu = new LR35902(memory_bus, interrupt);

    auto *lcd = new Display(memory_bus, interrupt);

    auto *render = new Render(lcd);
    render->init(cartridge->getTitle());

//    4194304  // clock cycles per second
//    59.73 hz - refresh rate
//    1 / 59.73 = 0.016742
//    4194304 * 0.16742 = 70221
    const int MAX_CYCLES = 70221;
//5016
    while (running) {
        int cyclesThisUpdate = 0;
        while(cyclesThisUpdate <= MAX_CYCLES) {
            // start
            int cycles = cpu->check_interrupts();
            cycles += cpu->execute_cycle();
            cyclesThisUpdate += cycles;
            lcd->UpdateGraphics(cycles);
            timer->doTimers(cycles);
        }

        render->renderScreen();

        while(SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            }
            if (e.type == SDL_KEYDOWN)
            {
                switch (e.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    default:
                        break;
                }
            }
        }
    }

    return 0;
}