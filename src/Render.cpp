#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif

#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif

#include <glad/glad.h>
#include <iostream>
#include "Render.h"

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

Render::Render(Display * lcd) {
    display = lcd;
}

void Render::init(std::string title) {
    scaling_factor = 6;

    uint32_t screen_width = sys_width * scaling_factor;
    uint32_t screen_height = sys_height * scaling_factor;

    int32_t win_width = screen_width;
    int32_t win_height = screen_height;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screen_width, screen_height, SDL_WINDOW_OPENGL);
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

void Render::renderScreen() {
    glBindTexture(GL_TEXTURE_2D, lcd_texture);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sys_width, sys_height, 0, GL_RGB, GL_UNSIGNED_BYTE, display->getScreenData());
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
