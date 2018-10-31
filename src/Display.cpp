#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include <glad/glad.h>
#include "Display.h"

#define SIMPLE_VERTEX_SHADER "#version 330 core \n \
layout (location = 0) in vec3 aPos; \n \
layout (location = 1) in vec2 aTexCoord; \n \
out vec2 TexCoord; \n \
void main() { \n \
    gl_Position = vec4(aPos, 1.0f); \n \
    TexCoord = vec2(aTexCoord.x, aTexCoord.y); \n \
}\n\0"

#define SIMPLE_FRAGMENT_SHADER "#version 330 core \n \
out vec4 FragColor; \n \
in vec3 ourColor; \n \
in vec2 TexCoord; \n \
uniform sampler2D screen_texture; \n \
void main() { \n \
    FragColor = texture(screen_texture, TexCoord); \n \
}\n\0"


void Display::init(LR35902 *cpu, MemoryBus *m) {
    scaling_factor = 4;

    uint32_t screen_width = sys_width * scaling_factor;
    uint32_t screen_height = sys_height * scaling_factor;

    int32_t win_width = screen_width;
    int32_t win_height = screen_height;

    c = cpu;
    memory = m;

    retrace_ly = RETRACE_START;
    vblankcount = 0;
    hack = 0;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow("Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height,
                              SDL_WINDOW_OPENGL);
    SDL_GetWindowSize(window, &win_width, &win_height);
    glcontext = SDL_GL_CreateContext(window);

    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    const GLchar *vertexShaderSource = SIMPLE_VERTEX_SHADER;

    GLuint vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const GLchar *fragmentShaderSource = SIMPLE_FRAGMENT_SHADER;

    GLuint fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    GLfloat vertices[] = {
        1.0f, 1.0f, 0.0f, 1.0, 0.0f,
        1.0f, -1.0f, 0.0f, 1.0, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0, 1.0f,
        -1.0f, 1.0f, 0.0f, 0.0, 0.0f
    };

    GLint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    glGenTextures(1, &lcd_texture);
}

void Display::renderScreen() {
    glBindTexture(GL_TEXTURE_2D, lcd_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sys_width, sys_height, 0, GL_RGB, GL_UNSIGNED_BYTE, screenData);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lcd_texture);
    glUniform1i(glGetUniformLocation(shaderProgram, "screen_texture"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glUseProgram(0);

    SDL_GL_SwapWindow(window);
}

// Move these somewhere useful
bool test_bit(uint8_t byte, int bit) {
    return (byte >> bit) & 1;
}

void Display::setLCDMode() {
    int currentMode = getMode();

    bool requireInterrupt = false;

    // LCD is in V-Blank. Mode 1
    if (getLY() >= VERTICAL_BLANK_SCAN_LINE) {
        setMode(V_BLANK);
        requireInterrupt = isVBlankInterruptEnabled();
    } else {
        int mode2bounds = RETRACE_START - 80;
        int mode3bounds = mode2bounds - 172;

        if (retrace_ly >= mode2bounds) {
            setMode(OAM_RAM);
            requireInterrupt = isOAMInterruptEnabled();
        } else if (retrace_ly >= mode3bounds) {
            setMode(TRANSFER);
        } else {
            setMode(H_BLANK);
            requireInterrupt = isHBlankInterruptEnabled();
        }
    }

    if (requireInterrupt && (currentMode != getMode())) {
        c->request_interrupt(1);
    }

    if (getLY() == getLYC()) {
        setCoincidenceFlag(true);
        if (isCoincidenceInterruptEnabled()) {
            c->request_interrupt(1);
        }
    } else {
        setCoincidenceFlag(false);
    }
}

void Display::UpdateGraphics(int cycles) {
    if (!isLCDEnabled()) {
        retrace_ly = RETRACE_START;
        memory->reset_LY();
        setMode(V_BLANK);
        return;
    }

    setLCDMode();

    retrace_ly -= cycles;

    if (retrace_ly <= 0) {
        retrace_ly = RETRACE_START;
        drawCurrentLine();
    }
}

void Display::drawCurrentLine() {
    if (getLY() == VERTICAL_BLANK_SCAN_LINE) {
        issueVerticalBlank();
    }

    if (getLY() < VERTICAL_BLANK_SCAN_LINE) {
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
    vblankcount++;
    c->request_interrupt(0);
    if (hack == 60) {
        vblankcount = 0;
    }
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
    if (!using_window) {
        yPos = scrollY + getLY();
    } else {
        yPos = getLY() - windowY;
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
                red = 0x9B;
                green = 0xBC;
                blue = 0x0F;
                break;
            case 1:
                red = 0x8B;
                green = 0xAC;
                blue = 0x0F;
                break;
            case 2:
                red = 0x30;
                green = 0x62;
                blue = 0x30;
                break;
            default:
                red = 0x0F;
                green = 0x38;
                blue = 0x0F;
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
        uint8_t index = sprite * 4;
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
            uint8_t data1 = memory->read_byte((0x8000 + (tileLocation * 16)) + line);
            uint8_t data2 = memory->read_byte((0x8000 + (tileLocation * 16)) + line + 1);


            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
                int colourbit = tilePixel;
                if (xFlip) {
                    colourbit -= 7;
                    colourbit *= -1;
                }
                int colourNum = test_bit(data2, colourbit);
                colourNum <<= 1;
                colourNum |= test_bit(data1, colourbit);

                int col = getColour(colourNum, test_bit(attributes, 4) ? OBP1_REGISTER : OBP0_REGISTER);

                // white is transparent for sprites.
                if (col == 0)
                    continue;

                uint8_t red;
                uint8_t green;
                uint8_t blue;

                switch (col) {
                    case 1:
                        red = 0x8B;
                        green = 0xAC;
                        blue = 0x0F;
                        break;
                    case 2:
                        red = 0x30;
                        green = 0x62;
                        blue = 0x30;
                        break;
                    case 3:
                        red = 0x0F;
                        green = 0x38;
                        blue = 0x0F;
                        break;
                    default:
                        break;
                }

                int xPix = 0 - tilePixel;
                xPix += 7;

                int pixel = xPos + xPix;

                if ((scanline < 0) || (scanline > 143) || (pixel < 0) || (pixel > 159)) {
                    //	assert(false) ;
                    continue;
                }

                // check if pixel is hidden behind background
                if (test_bit(attributes, 7) == 1) {
                    if ((screenData[pixel][scanline][0] != 255) || (screenData[pixel][scanline][1] != 255) ||
                        (screenData[pixel][scanline][2] != 255))
                        continue;
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

void Display::setCoincidenceFlag(bool setBit) {
    if (setBit) {
        memory->write_byte(STAT_REGISTER, memory->read_byte(STAT_REGISTER) | 1 << 2);
    } else {
        memory->write_byte(STAT_REGISTER, memory->read_byte(STAT_REGISTER) & ~(1 << 2));
    }
}


uint8_t Display::getLY() {
    return memory->read_byte(LY_REGISTER);
}

uint8_t Display::getLYC() {
    return memory->read_byte(LYC_REGISTER);
}

// LCDC_REGISTER
bool Display::isLCDEnabled() {
    return memory->read_byte(LCDC_REGISTER) >> 7;
}

bool Display::getWindowTileMapDisplaySelect() {
    return (memory->read_byte(LCDC_REGISTER) >> 6) & 1;
}

bool Display::isWindowDisplayEnabled() {
    return (memory->read_byte(LCDC_REGISTER) >> 5) & 1;
}

bool Display::getBGAndWindowTileDataSelect() {
    return (memory->read_byte(LCDC_REGISTER) >> 4) & 1;
}

bool Display::getBGTileMapDisplaySelect() {
    return (memory->read_byte(LCDC_REGISTER) >> 3) & 1;
}

bool Display::getObjSize() {
    return (memory->read_byte(LCDC_REGISTER) >> 2) & 1;
}

bool Display::isObjDisplayEnabled() {
    return (memory->read_byte(LCDC_REGISTER) >> 1) & 1;
}

bool Display::isBGDDislayEnabled() {
    return memory->read_byte(LCDC_REGISTER) & 1;
}

// STAT_REGISTER
bool Display::isCoincidenceInterruptEnabled() {
    return (memory->read_byte(STAT_REGISTER) >> 6) & 1;
}

bool Display::isOAMInterruptEnabled() {
    return (memory->read_byte(STAT_REGISTER) >> 5) & 1;
}

bool Display::isVBlankInterruptEnabled() {
    return (memory->read_byte(STAT_REGISTER) >> 4) & 1;
}

bool Display::isHBlankInterruptEnabled() {
    return (memory->read_byte(STAT_REGISTER) >> 3) & 1;
}

int Display::getMode() {
    return memory->read_byte(STAT_REGISTER) & 3;
}
