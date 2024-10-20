#include <stdlib.h>
#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "../include/3d-engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

int main(int argc, char* argv[]) {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return -1;
  }

  // Create an SDL window with OpenGL
  SDL_Window* window = SDL_CreateWindow("Graphics engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIEWWIDTH, VIEWHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_GL_SetSwapInterval(1);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }

  Model heightmap = createHeightmapModel("heightmap.png");

  glViewport(0, 0, VIEWWIDTH, VIEWHEIGHT);

  GLuint VAO = createVAO();
  GLuint VBO = createVBO();
  GLuint EBO = createEBO();

  bindVAO(VAO);
  bindVBO(VBO, heightmap.sizeVertices, heightmap.vertices);
  bindEBO(EBO, heightmap.sizeIndices, heightmap.indices);

  enableVertexAttr();

  int shaderProgram = createShader();

  float x = 0.0, y = 0.0, z = -3.0;
  vec3 cameraPos = {x,y,z};
  float yaw = 0.0;
  float pitch = 0.0;

  mat4 view, projection;

  glm_translate_make(view, (vec3){x, y, z});
  glm_perspective(glm_rad(45.0f), VIEWWIDTH / VIEWHEIGHT, 0.1f, 1000.0f, projection);

  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int projLoc = glGetUniformLocation(shaderProgram, "projection");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float *)view);
  glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const float *)projection);


  int quit = 0;
  SDL_Event event;

  const Uint8 *keys = SDL_GetKeyboardState(NULL);

  while (!quit) {
    handleEvents(keys, &quit, &yaw, &pitch, cameraPos, event);

    glm_mat4_identity(view);
    glm_rotate(view, glm_rad(pitch), (vec3){1.0f, 0.0f, 0.0f});
    glm_rotate(view, glm_rad(yaw), (vec3){0.0f, 1.0f, 0.0f});
    glm_translate(view, cameraPos);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Set the background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(shaderProgram);

    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const float *)view);
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, (const float *)projection);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, heightmap.sizeIndices, GL_UNSIGNED_INT, 0);
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
