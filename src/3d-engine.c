#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "../include/shaders.h"

float vertices[] = {
    0.5f,  0.5f,  0.5f,  // front top right
    0.5f, -0.5f,  0.5f,  // front bottom right
   -0.5f, -0.5f,  0.5f,  // front bottom left
   -0.5f,  0.5f,  0.5f,  // front top left
    0.5f,  0.5f, -0.5f,  // back top right
    0.5f, -0.5f, -0.5f,  // back bottom right
   -0.5f, -0.5f, -0.5f,  // back bottom left
   -0.5f,  0.5f, -0.5f   // back top left
};

unsigned int indices[] = {
    // Front face
    0, 1, 3,
    1, 2, 3,

    // Back face
    4, 5, 7,
    5, 6, 7,

    // Left face
    3, 2, 7,
    2, 6, 7,

    // Right face
    0, 1, 4,
    1, 5, 4,

    // Top face
    3, 0, 4,
    3, 4, 7,

    // Bottom face
    1, 2, 6,
    1, 6, 5
};

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  // Create an SDL window with OpenGL
  SDL_Window* window = SDL_CreateWindow("Graphics engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  SDL_GL_SetSwapInterval(1);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }

  glViewport(0, 0, 640, 480);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  

  int shaderProgram = createMesh();

  float x, y = 0.0;
  float z = -3.0;

  mat4 view, projection;

  glm_translate_make(view, (vec3){x, y, z});
  glm_perspective(glm_rad(45.0f), (float)640 / (float)480, 0.1f, 100.0f, projection);

  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float *)view);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const float *)projection);

  int quit = 0;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      switch (event.type) {
        case SDL_QUIT:
          quit = 1;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_w:
              z+=SPEED;
              break;
            case SDLK_a:
              x+=SPEED;
              break;
            case SDLK_s:
              z-=SPEED;
              break;
            case SDLK_d:
              x-=SPEED;
              break;
              
            case SDLK_q:
              y+=SPEED;
              break;
            case SDLK_e:
              y-=SPEED;
              break;
          }
      }
    }

    glm_translate_make(view, (vec3){x, y, z});

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Set the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float *)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const float *)projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
