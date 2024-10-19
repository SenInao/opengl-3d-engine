#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>

#define SPEED 1.0f
#define SENS 0.1f

#define VIEWHEIGHT 600.0f
#define VIEWWIDTH 900.0f


typedef struct Vertex {
  float x;
  float y;
  float z;
} Vertex;

typedef struct Model {
  Vertex *vertices;
  int *indices;
  int sizeVertices;
  int sizeIndices;
} Model;

Model createHeightmapModel(char *filename);
int createShader();
void handleEvents(const Uint8 *keys, int *quit, float *yaw, float *pitch, vec3 cameraPos, SDL_Event event);
