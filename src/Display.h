#pragma once

#ifndef EMULATOR_LCD_H
#define EMULATOR_LCD_H

#include <cstdint>
#include <SDL.h>
#include <SDL_opengl.h>
#include "MemoryBus.h"
#include "Interrupt.h"

#define LCDC_REGISTER 0xFF40
#define STAT_REGISTER 0xFF41

#define SCY_REGISTER 0xFF42
#define SCX_REGISTER 0xFF43

#define LY_REGISTER 0xFF44
#define LYC_REGISTER 0xFF45

#define DMA_REGISTER 0xFF46

#define BGP_REGISTER 0xFF47
#define OBP0_REGISTER 0xFF48
#define OBP1_REGISTER 0xFF49

#define WY_REGISTER 0xFF4A
#define WX_REGISTER 0xFF4B

#define VERTICAL_BLANK_SCAN_LINE 0x90
#define CLOCKS_PER_LINE 456

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

class Display {
public:
    explicit Display(MemoryBus*, Interrupt*);
    void UpdateGraphics(int);
    uint8_t* getScreenData();

    enum {
        WHITE,
        LIGHT_GREY,
        DARK_GREY,
        BLACK
    };

private:
    enum {
        H_BLANK,
        V_BLANK,
        OAM_RAM,
        TRANSFER
    };

    MemoryBus* memory;
    Interrupt* interrupt;

    uint8_t screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

    int retrace_ly;

    // LCDC_REGISTER
    bool isLCDEnabled();
    bool getWindowTileMapDisplaySelect();
    bool isWindowDisplayEnabled();
    bool getBGAndWindowTileDataSelect();
    bool getBGTileMapDisplaySelect();
    bool getObjSize();
    bool isObjDisplayEnabled();
    bool isBGDDislayEnabled();

    void setLCDEnabled(bool);
    void setWindowTileMapDisplaySelect(bool);
    void setWindowDisplayEnabled(bool);
    void setBGAndWindowTileDataSelect(bool);
    void setBGTileMapDisplaySelect(bool);
    void setObjSize(bool);
    void setObjDisplayEnabled(bool);
    void setBGDDislayEnabled(bool);

    // STAT_REGISTER
    bool isCoincidenceInterruptEnabled();
    bool isOAMInterruptEnabled();
    bool isVBlankInterruptEnabled();
    bool isHBlankInterruptEnabled();
    bool isCoincidence();
    int getMode();

    void setCoincidenceEnabled(bool);
    void setOAMInterrupt(bool);
    void setVBlankInterrupt(bool);
    void setHBlankInterrupt(bool);
    void setCoincidenceFlag();
    void resetCoincidenceFlag();
    void setMode(int); // Should be read only

    uint8_t getSCY();
    uint8_t getSCX();
    uint8_t getWY();
    uint8_t getWX();
    uint8_t getLY();
    uint8_t getLYC();
    void setLCDMode();
    void drawCurrentLine();
    void drawScanLine();
    void issueVerticalBlank();
    void renderBackground();
    void renderSprites();
    int getColour(int, uint16_t);
};

#endif //EMULATOR_LCD_H
