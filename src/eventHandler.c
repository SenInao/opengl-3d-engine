#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <cglm/cglm.h>
#include "../include/3d-engine.h"

void handleEvents(const Uint8 *keys, int *quit, float *yaw, float *pitch, vec3 cameraPos, SDL_Event event) {
  if (keys[SDL_SCANCODE_W]) {
    cameraPos[0] -= SPEED * sin(glm_rad(*yaw));
    cameraPos[2] += SPEED * cos(glm_rad(*yaw));
  }
  if (keys[SDL_SCANCODE_S]) {
    cameraPos[0] += SPEED * sin(glm_rad(*yaw));
    cameraPos[2] -= SPEED * cos(glm_rad(*yaw));
  }
  if (keys[SDL_SCANCODE_A]) {
    cameraPos[0] += SPEED * cos(glm_rad(*yaw));
    cameraPos[2] += SPEED * sin(glm_rad(*yaw));
  }
  if (keys[SDL_SCANCODE_D]) {
    cameraPos[0] -= SPEED * cos(glm_rad(*yaw));
    cameraPos[2] -= SPEED * sin(glm_rad(*yaw));
  }
  if (keys[SDL_SCANCODE_Q]) {
    cameraPos[1] += SPEED;
  }
  if (keys[SDL_SCANCODE_E]) {
    cameraPos[1] -= SPEED;
  }

  while (SDL_PollEvent(&event) != 0) {
    switch (event.type) {
      case SDL_QUIT:
        *quit = 1;
        break;

      case SDL_MOUSEMOTION:
        *yaw+= event.motion.xrel * SENS;
        *pitch+= event.motion.yrel * SENS;

        if (*pitch > 89.0f) {
          *pitch = 89.0f;
        }
        if (*pitch < -89.0f) {
          *pitch = -89.0f;
        }
        break;
    }
  }
}
