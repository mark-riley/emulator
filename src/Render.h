#pragma once

#ifndef EMULATOR_RENDER_H
#define EMULATOR_RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <string>
#include "Display.h"

#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 144

class Render {
public:
    SDL_Window* window;
    SDL_GLContext glcontext;

    explicit Render(Display*);
    void init(std::string);
    void renderScreen();

private:
    Display * display;
    GLuint VAO;
    GLuint shaderProgram;
    GLuint lcd_texture;
    GLuint VBO, EBO;
    uint32_t sys_width = SCREEN_WIDTH;
    uint32_t sys_height = SCREEN_HEIGHT;
    uint8_t scaling_factor;
};


#endif //EMULATOR_RENDER_H
