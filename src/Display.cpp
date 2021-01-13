#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include "Display.h"

Display::Display(MemoryBus* m, Interrupt* i) {
    memory = m;
    interrupt = i;

    retrace_ly = 0;
}

// Move these somewhere useful
bool test_bit(uint8_t byte, int bit) {
    return (byte >> bit) & 1;
}

uint8_t set_bit(uint8_t byte, int bit) {
    return byte | 1 << bit;
}

uint8_t reset_bit(uint8_t byte, int bit) {
    return byte & ~(1 << bit);
}

uint8_t* Display::getScreenData() {
    return (uint8_t*)screenData;
}

void Display::UpdateGraphics(int cycles) {
    setLCDMode();

    if (isLCDEnabled()) {
        retrace_ly += cycles;

        if (retrace_ly >= CLOCKS_PER_LINE) {
            retrace_ly -= CLOCKS_PER_LINE;
            drawCurrentLine();
        }
    }
}

void Display::setLCDMode() {
    if (!isLCDEnabled()) {
        retrace_ly = 0;
        memory->reset_LY();
        setMode(V_BLANK);
        return;
    }

    int currentMode = getMode();
    bool requireInterrupt = false;

    if (getLY() >= VERTICAL_BLANK_SCAN_LINE) {
        // Mode 1
        // LY is in the V Blank range. The only mode we can be in is 1 (V_BLANK)
        // CPU will be able to access display RAM.
        setMode(V_BLANK);
        requireInterrupt = isVBlankInterruptEnabled();
    } else {
        // We are scanning the individual lines.
        if (retrace_ly < 204) {
            // Mode 0
            // CPU will be able to access display RAM.
            setMode(H_BLANK);
            requireInterrupt = isHBlankInterruptEnabled();
        } else if (retrace_ly < 284) {
            // Mode 2
            // CPU cannot access OAM.
            setMode(OAM_RAM);
            requireInterrupt = isOAMInterruptEnabled();
        } else {
            // Mode 3
            // CPU cannot access OAM nor display RAM.
            setMode(TRANSFER);
        }
    }

    if (requireInterrupt && (currentMode != getMode())) {
        interrupt->request_interrupt(interrupt->STAT);
    }

    if (getLY() == getLYC()) {
        setCoincidenceFlag();
        if (isCoincidenceInterruptEnabled()) {
            interrupt->request_interrupt(interrupt->STAT);
        }
    } else {
        resetCoincidenceFlag();
    }
}

void Display::drawCurrentLine() {
    if (getLY() == VERTICAL_BLANK_SCAN_LINE) {
        issueVerticalBlank();
    } else if (getLY() < VERTICAL_BLANK_SCAN_LINE) {
        drawScanLine();
    }
    memory->increment_LY();
}

void Display::drawScanLine() {
    if (isBGDDislayEnabled()) {
        renderBackground();
    }
    if (isObjDisplayEnabled()) {
        renderSprites();
    }
}

void Display::issueVerticalBlank() {
    interrupt->request_interrupt(interrupt->V_BLANK);
}

uint8_t Display::getSCY() {
    return memory->read_byte(SCY_REGISTER);
}

uint8_t Display::getSCX() {
    return memory->read_byte(SCX_REGISTER);
}

uint8_t Display::getWY() {
    return memory->read_byte(WY_REGISTER);
}

uint8_t Display::getWX() {
    return memory->read_byte(WX_REGISTER) - 0x07;
}

void Display::renderBackground() {
    uint16_t tileData = 0;
    uint16_t backgroundMemory = 0;
    bool unsig = true;

    uint8_t scrollX = getSCX();
    uint8_t scrollY = getSCY();
    uint8_t windowX = getWX();
    uint8_t windowY = getWY();

    bool using_window = false;

    if (isWindowDisplayEnabled()) {
        if (windowY <= getLY()) {
            using_window = true;
        }
    }

    if (getBGAndWindowTileDataSelect()) {
        tileData = 0x8000;
    } else {
        tileData = 0x8800;
        unsig = false;
    }

    if (!using_window) {
        if (getBGTileMapDisplaySelect()) {
            backgroundMemory = 0x9C00;
        } else {
            backgroundMemory = 0x9800;
        }
    } else {
        if (getWindowTileMapDisplaySelect()) {
            backgroundMemory = 0x9C00;
        } else {
            backgroundMemory = 0x9800;
        }
    }

    uint8_t yPos = 0;
    if (using_window) {
        yPos = getLY() - windowY;
    } else {
        yPos = scrollY + getLY();
    }
    uint16_t tileRow = (((uint8_t) (yPos / 8)) * 32);

    for (uint8_t pixel = 0; pixel < 160; pixel++) {
        uint8_t xPos = pixel + scrollX;
        if (using_window) {
            if (pixel >= windowX) {
                xPos = pixel - windowX;
            }
        }

        uint16_t tileCol = (xPos / 8);
        int16_t tileNum;

        if (unsig) {
            tileNum = (uint8_t) memory->read_byte(backgroundMemory + tileRow + tileCol);
        } else {
            tileNum = (int8_t) memory->read_byte(backgroundMemory + tileRow + tileCol);
        }

        uint16_t tileLocation = tileData;

        if (unsig) {
            tileLocation += (tileNum * 16);
        } else {
            tileLocation += ((tileNum + 128) * 16);
        }

        uint8_t line = yPos % 8;
        line *= 2;

        uint8_t data1 = memory->read_byte(tileLocation + line);
        uint8_t data2 = memory->read_byte(tileLocation + line + 1);

        int colourBit = xPos % 8;
        colourBit -= 7;
        colourBit *= -1;

        int colourNum = test_bit(data2, colourBit);
        colourNum <<= 1;
        colourNum |= test_bit(data1, colourBit);

        int col = getColour(colourNum, BGP_REGISTER);
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        switch (col) {
            case 0:
                red = 0xFF;
                green = 0xFF;
                blue = 0xFF;
                break;
            case 1:
                red = 0xCC;
                green = 0xCC;
                blue = 0xCC;
                break;
            case 2:
                red = 0x66;
                green = 0x66;
                blue = 0x66;
                break;
            default:
                red = 0x00;
                green = 0x00;
                blue = 0x00;
                break;
        }

        screenData[getLY()][pixel][0] = red;
        screenData[getLY()][pixel][1] = green;
        screenData[getLY()][pixel][2] = blue;
    }
}

int Display::getColour(int colourNum, uint16_t address) {
    uint8_t palette = memory->read_byte(address);
    return (palette >> (colourNum * 2)) & 0x3;
}

void Display::renderSprites() {
    bool use8x16 = false;
    if (getObjSize())
        use8x16 = true;

    for (int sprite = 0; sprite < 40; sprite++) {
        uint8_t index = (40 - sprite) * 4;
        uint8_t yPos = memory->read_byte(0xFE00 + index) - 16;
        uint8_t xPos = memory->read_byte(0xFE00 + index + 1) - 8;
        uint8_t tileLocation = memory->read_byte(0xFE00 + index + 2);
        uint8_t attributes = memory->read_byte(0xFE00 + index + 3);

        bool yFlip = test_bit(attributes, 6);
        bool xFlip = test_bit(attributes, 5);

        int scanline = getLY();

        int ysize = 8;

        if (use8x16)
            ysize = 16;

        if ((scanline >= yPos) && (scanline < (yPos + ysize))) {
            int line = scanline - yPos;

            if (yFlip) {
                line -= ysize;
                line *= -1;
            }

            line *= 2;
            uint16_t data_address = (0x8000 + (tileLocation * 16)) + line;
            uint8_t data1 = memory->read_byte(data_address);
            uint8_t data2 = memory->read_byte(data_address + 1);

            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
                int colourbit = tilePixel;
                if (xFlip) {
                    colourbit -= 7;
                    colourbit *= -1;
                }
                int colourNum = test_bit(data2, colourbit);
                colourNum <<= 1;
                colourNum |= test_bit(data1, colourbit);

                uint16_t colour_address = test_bit(attributes, 4) ? OBP1_REGISTER : OBP0_REGISTER;
                int col = getColour(colourNum, colour_address);

                // white is transparent for sprites.
                if (col == WHITE)
                    continue;

                uint8_t red;
                uint8_t green;
                uint8_t blue;

                switch (col) {
                    case 1:
                        red = 0xCC;
                        green = 0xCC;
                        blue = 0xCC;
                        break;
                    case 2:
                        red = 0x66;
                        green = 0x66;
                        blue = 0x66;
                        break;
                    case 3:
                        red = 0x00;
                        green = 0x00;
                        blue = 0x00;
                        break;
                    default:
                        break;
                }

                int xPix = 0 - tilePixel;
                xPix += 7;

                int pixel = xPos + xPix;

                if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159)) {
                    continue;
                }

                // check if pixel is hidden behind background
                if (test_bit(attributes, 7) == 1) {
                    if ((screenData[pixel][scanline][0] != 255) || (screenData[pixel][scanline][1] != 255) || (screenData[pixel][scanline][2] != 255)) {
                        continue;
                    }
                }

                screenData[scanline][pixel][0] = red;
                screenData[scanline][pixel][1] = green;
                screenData[scanline][pixel][2] = blue;
            }
        }
    }
}

void Display::setMode(int mode) {
    uint8_t stat = memory->read_byte(STAT_REGISTER);
    stat |= mode;
    memory->write_byte(STAT_REGISTER, stat);
}

void Display::setCoincidenceFlag() {
    memory->write_byte(STAT_REGISTER, set_bit(memory->read_byte(STAT_REGISTER), 2));
}

void Display::resetCoincidenceFlag() {
    memory->write_byte(STAT_REGISTER, reset_bit(memory->read_byte(STAT_REGISTER), 2));
}

uint8_t Display::getLY() {
    return memory->read_byte(LY_REGISTER);
}

uint8_t Display::getLYC() {
    return memory->read_byte(LYC_REGISTER);
}

// LCDC_REGISTER
bool Display::isLCDEnabled() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 7);
}

bool Display::getWindowTileMapDisplaySelect() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 6);
}

bool Display::isWindowDisplayEnabled() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 5);
}

bool Display::getBGAndWindowTileDataSelect() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 4);
}

bool Display::getBGTileMapDisplaySelect() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 3);
}

bool Display::getObjSize() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 2);
}

bool Display::isObjDisplayEnabled() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 1);
}

bool Display::isBGDDislayEnabled() {
    return test_bit(memory->read_byte(LCDC_REGISTER), 0);
}

// STAT_REGISTER
bool Display::isCoincidenceInterruptEnabled() {
    return test_bit(memory->read_byte(STAT_REGISTER), 6);
}

bool Display::isOAMInterruptEnabled() {
    return test_bit(memory->read_byte(STAT_REGISTER), 5);
}

bool Display::isVBlankInterruptEnabled() {
    return test_bit(memory->read_byte(STAT_REGISTER), 4);
}

bool Display::isHBlankInterruptEnabled() {
    return test_bit(memory->read_byte(STAT_REGISTER), 3);
}

int Display::getMode() {
    return memory->read_byte(STAT_REGISTER) & 3;
}
