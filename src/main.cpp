#include "file-utils.h"
#include "Gameboy.h"

#define BOOT_ROM "bios/dmg_boot.bin"

//#define GAME_ROM "games/Alleyway (World).gb"
//#define GAME_ROM "games/Baseball (World).gb"
//#define GAME_ROM "games/Super Mario Land (World).gb"
#define GAME_ROM "games/Tennis (World).gb"
//#define GAME_ROM "games/Tetris (World) (Rev A).gb"
//#define GAME_ROM "games/Yakuman (Japan).gb"

//#define GAME_ROM "games/Dr. Mario (World).gb"

//#define GAME_ROM "dmg-acid2.gb"

//#define GAME_ROM "blargg/halt_bug.gb" // Fails 3db103c3

//#define GAME_ROM "blargg/cgb_sound/cgb_sound.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/01-registers.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/02-len ctr.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/03-trigger.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/04-sweep.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/05-sweep details.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/06-overflow on trigger.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/07-len sweep period sync.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/08-len ctr during power.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/09-wave read while on.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/10-wave trigger while on.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/11-regs after power.gb" // Not implemented
//#define GAME_ROM "blargg/cgb_sound/rom_singles/12-wave.gb" // Not implemented

//#define GAME_ROM "blargg/cpu_instrs/cpu_instrs.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/01-special.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/02-interrupts.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/03-op sp,hl.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/04-op r,imm.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/05-op rp.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/06-ld r,r.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/07-jr,jp,call,ret,rst.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/08-misc instrs.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/09-op r,r.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/10-bit ops.gb" // Passes
//#define GAME_ROM "blargg/cpu_instrs/individual/11-op a,(hl).gb" // Passes

//#define GAME_ROM "blargg/dmg_sound/dmg_sound.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/01-registers.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/02-len ctr.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/03-trigger.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/04-sweep.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/05-sweep details.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/06-overflow on trigger.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/07-len sweep period sync.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/08-len ctr during power.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/09-wave read while on.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/10-wave trigger while on.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/11-regs after power.gb" // Not implemented
//#define GAME_ROM "blargg/dmg_sound/rom_singles/12-wave write while on.gb" // Not implemented

//#define GAME_ROM "blargg/instr_timing/instr_timing.gb" // Fails 255

//#define GAME_ROM "blargg/interrupt_time/interrupt_time.gb" // Fails

//#define GAME_ROM "blargg/mem_timing/mem_timing.gb" // Fails
//#define GAME_ROM "blargg/mem_timing/individual/01-read_timing.gb" // Fails 30
//#define GAME_ROM "blargg/mem_timing/individual/02-write_timing.gb" // Fails 15
//#define GAME_ROM "blargg/mem_timing/individual/03-modify_timing.gb" // Fails 16+
//#define GAME_ROM "blargg/mem_timing-2/mem_timing.gb" // Fails
//#define GAME_ROM "blargg/mem_timing-2/rom_singles/01-read_timing.gb" // Fails 30
//#define GAME_ROM "blargg/mem_timing-2/rom_singles/02-write_timing.gb"// Fails 15
//#define GAME_ROM "blargg/mem_timing-2/rom_singles/03-modify_timing.gb" // Fails 16+

//#define GAME_ROM "blargg/oam_bug/oam_bug.gb" // Fails
//#define GAME_ROM "blargg/oam_bug/rom_singles/1-lcd_sync.gb" // Fails (LCD starts too early)
//#define GAME_ROM "blargg/oam_bug/rom_singles/2-causes.gb" // Fails LD DE $FE00 : INC DE
//#define GAME_ROM "blargg/oam_bug/rom_singles/3-non_causes.gb" // Passes
//#define GAME_ROM "blargg/oam_bug/rom_singles/4-scanline_timing.gb" // Fails INC DE at first Corruption
//#define GAME_ROM "blargg/oam_bug/rom_singles/5-timing_bug.gb" // Fails Should corrupt at beginning of first scanline
//#define GAME_ROM "blargg/oam_bug/rom_singles/6-timing_no_bug.gb" // Passes
//#define GAME_ROM "blargg/oam_bug/rom_singles/7-timing_effect.gb" // Fails 00000000
//#define GAME_ROM "blargg/oam_bug/rom_singles/8-instr_effect.gb" // Fails 00000000 INC/DEV rp pattern is wrong

//#define GAME_ROM "mooneye/acceptance/add_sp_e_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/boot_div2-S.gb" // Fails 6
//#define GAME_ROM "mooneye/acceptance/boot_div-dmg0.gb" // Fails 6
//#define GAME_ROM "mooneye/acceptance/boot_div-dmgABCmgb.gb" // Fails 6
//#define GAME_ROM "mooneye/acceptance/boot_div-S.gb" // Fails 6
//#define GAME_ROM "mooneye/acceptance/boot_hwio-dmg0.gb" // Fails - expected CF, got 00
//#define GAME_ROM "mooneye/acceptance/boot_hwio-dmgABCmgb.gb" // Fails - expected CF, got 00
//#define GAME_ROM "mooneye/acceptance/boot_hwio-S.gb" // Fails - expected FF, got 00
//#define GAME_ROM "mooneye/acceptance/boot_regs-dmg0.gb" // Fails 5
//#define GAME_ROM "mooneye/acceptance/boot_regs-dmgABC.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/boot_regs-mgb.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/boot_regs-sgb.gb" // Fails 5
//#define GAME_ROM "mooneye/acceptance/boot_regs-sgb2.gb" // Fails 6
//#define GAME_ROM "mooneye/acceptance/call_cc_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/call_cc_timing2.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/call_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/call_timing2.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/di_timing-GS.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/div_timing.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/ei_sequence.gb" // Fails NO INTR
//#define GAME_ROM "mooneye/acceptance/ei_timing.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/halt_ime0_ei.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/halt_ime0_nointr_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/halt_ime1_timing.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/halt_ime1_timing2-GS.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/if_ie_registers.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/intr_timing.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/jp_cc_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/jp_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ld_hl_sp_e_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/oam_dma_restart.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/oam_dma_start.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/oam_dma_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/pop_timing.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/push_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/rapid_di_ei.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/ret_cc_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/reti_intr_timing.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/reti_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ret_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/rst_timing.gb" // Fails (Blank Screen)

//#define GAME_ROM "mooneye/acceptance/bits/mem_oam.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/bits/reg_f.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/bits/unused_hwio-GS.gb" // Fails Wrote 00111111 expected 11 got 00

//#define GAME_ROM "mooneye/acceptance/instr/daa.gb" // Passes

//#define GAME_ROM "mooneye/acceptance/interrupts/ie_push.gb" // Fails R1 not cancelled

//#define GAME_ROM "mooneye/acceptance/oam_dma/basic.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/oam_dma/reg_read.gb" // Fails r1
//#define GAME_ROM "mooneye/acceptance/oam_dma/sources-GS.gb" // Fails FE00

//#define GAME_ROM "mooneye/acceptance/ppu/hblank_ly_scx_timing-GS.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_1_2_timing-GS.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_2_0_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_2_mode0_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_2_mode0_timing_sprites.gb" // Fails (Blank Screen - white)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_2_mode3_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/intr_2_oam_ok_timing.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/lcdon_timing-GS.gb" // Fails Cycle 00 expected 84 actual 07
//#define GAME_ROM "mooneye/acceptance/ppu/lcdon_write_timing-GS.gb" // Fails cycle 12 expected 00 actual 81
//#define GAME_ROM "mooneye/acceptance/ppu/stat_irq_blocking.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/stat_lyc_onoff.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/acceptance/ppu/vblank_stat_intr-GS.gb" // Fails (Blank Screen)

//#define GAME_ROM "mooneye/acceptance/serial/boot_sclk_align-dmgABCmgb.gb" // Not implemented

//#define GAME_ROM "mooneye/acceptance/timer/div_write.gb" // Passes
//#define GAME_ROM "mooneye/acceptance/timer/rapid_toggle.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/timer/tim00.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim00_div_trigger.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/timer/tim01.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim01_div_trigger.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim10.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim10_div_trigger.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim11.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tim11_div_trigger.gb" // Fails 1
//#define GAME_ROM "mooneye/acceptance/timer/tima_reload.gb" // Fails 2
//#define GAME_ROM "mooneye/acceptance/timer/tima_write_reloading.gb" // Fails 3
//#define GAME_ROM "mooneye/acceptance/timer/tma_write_reloading.gb" // Fails 2

//#define GAME_ROM "mooneye/emulator-only/mbc1/bits_bank1.gb" // Passes
//#define GAME_ROM "mooneye/emulator-only/mbc1/bits_bank2.gb" // Fails R1 initial BANK2
//#define GAME_ROM "mooneye/emulator-only/mbc1/bits_mode.gb" // Passes
//#define GAME_ROM "mooneye/emulator-only/mbc1/bits_ramg.gb" // Fails (Crash)
//#define GAME_ROM "mooneye/emulator-only/mbc1/multicart_rom_8Mb.gb" // Fails Bank number 10 expected 00 actual 10
//#define GAME_ROM "mooneye/emulator-only/mbc1/ram_64kb.gb" // Fails (Crash)
//#define GAME_ROM "mooneye/emulator-only/mbc1/ram_256kb.gb" // Fails (Crash)
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_1Mb.gb" // Fails Bank Number 09 Expected 01 Actual FF
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_2Mb.gb" // Fails Bank Number 11 Expected 01 Actual FF
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_4Mb.gb" // Passes
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_8Mb.gb" // Fails Bank Number 20 expected 21 Actual 01
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_16Mb.gb" // Fails Bank Number 20 Expected 21 Actual 01
//#define GAME_ROM "mooneye/emulator-only/mbc1/rom_512kb.gb" // Fails Bank Number 05 Expected 01 Actual 00

//#define GAME_ROM "mooneye/emulator-only/mbc2/bits_ramg.gb" // Fails r1 test failed 3eff RAM not disabled
//#define GAME_ROM "mooneye/emulator-only/mbc2/bits_romb.gb" // Fails r2 1FFF
//#define GAME_ROM "mooneye/emulator-only/mbc2/bits_unused.gb" // Passes
//#define GAME_ROM "mooneye/emulator-only/mbc2/ram.gb" // Fails (Crash)
//#define GAME_ROM "mooneye/emulator-only/mbc2/rom_1Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc2/rom_2Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc2/rom_512kb.gb" // Fails (Blank Screen)

//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_1Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_2Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_4Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_8Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_16Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_32Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_64Mb.gb" // Fails (Blank Screen)
//#define GAME_ROM "mooneye/emulator-only/mbc5/rom_512kb.gb" // Fails (Blank Screen)

//#define GAME_ROM "mooneye/manual-only/sprite_priority.gb" // Fails

//#define GAME_ROM "mooneye/misc/boot_div-A.gb" // Fails 6
//#define GAME_ROM "mooneye/misc/boot_div-cgb0.gb" // Fails 6
//#define GAME_ROM "mooneye/misc/boot_div-cgbABCDE.gb" // Fails 6
//#define GAME_ROM "mooneye/misc/boot_hwio-C.gb" // Fails Expected FF got 00
//#define GAME_ROM "mooneye/misc/boot_regs-A.gb" // Fails 7
//#define GAME_ROM "mooneye/misc/boot_regs-cgb.gb" // Fails 6
//#define GAME_ROM "mooneye/misc/bits/unused_hwio-C.gb" // Fails wrote 00111111 expected 11 got 00
//#define GAME_ROM "mooneye/misc/ppu/vblank_stat_intr-C.gb" // Fails (Blank Screen)

//#define GAME_ROM "mooneye/utils/bootrom_dumper.gb"
//#define GAME_ROM "mooneye/utils/dump_boot_hwio.gb"

//#define GAME_ROM "misc/dmg_test_prog_ver1.gb"
//#define GAME_ROM "misc/dycptest2.gb"
//#define GAME_ROM "misc/bgbtest.gb"
//#define GAME_ROM "misc/opus5.gb"
//#define GAME_ROM "misc/lyc.gb"
//#define GAME_ROM "misc/naughtyemu.gb"

int main (int argv, char** args) {
    // Loading the boot rom
    const char* filePath = BOOT_ROM;
    std::vector<uint8_t> fileBuf = readFileToBuffer(filePath);

    const char* rompath = GAME_ROM;
    std::vector<uint8_t> romBuf = readFileToBuffer(rompath);

    auto* cartridge = new Cartridge(romBuf);
    auto* gameboy = new Gameboy(cartridge, fileBuf);

    gameboy->powerOn();

    return 0;
}