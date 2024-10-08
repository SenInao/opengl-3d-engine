#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

float vertices[] = {
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
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

  int quit = 0;
  SDL_Event event;

  while (!quit) {
    while (SDL_PollEvent(&event) != 0) {
      if (event.type == SDL_QUIT) {
        quit = 1;
      }
    }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);  // Set the background color
    glClear(GL_COLOR_BUFFER_BIT);          // Clear the screen

    SDL_GL_SwapWindow(window);
  }

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0
  ;}
