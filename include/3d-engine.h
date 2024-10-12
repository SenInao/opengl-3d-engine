#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>

#define SPEED 0.02f
#define SENS 0.1f

#define VIEWHEIGHT 600
#define VIEWWIDTH 900

int createMesh();
void handleEvents(const Uint8 *keys, int *quit, float *yaw, float *pitch, vec3 cameraPos, SDL_Event event);
