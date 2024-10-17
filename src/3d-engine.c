#define SDL_MAIN_HANDLED

#include <SDL2/SDL.h>
#include <SDL2/SDL_keyboard.h>
#include <GL/glew.h>
#include <stdio.h>
#include <cglm/cglm.h>

#include "../include/3d-engine.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

typedef struct Vertex {
  float x;
  float y;
  float z;
} Vertex;

float vertices[] = {
    0.5f,  0.5f,  0.5f,  // front top right
    0.5f, -0.5f,  0.5f,  // front bottom right
   -0.5f, -0.5f,  0.5f,  // front bottom left
   -0.5f,  0.5f,  0.5f,  // front top left
    0.5f,  0.5f, -0.5f,  // back top right
    0.5f, -0.5f, -0.5f,  // back bottom right
   -0.5f, -0.5f, -0.5f,  // back bottom left
   -0.5f,  0.5f, -0.5f  // back top left
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
  SDL_Window* window = SDL_CreateWindow("Graphics engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, VIEWWIDTH, VIEWHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  SDL_GLContext glContext = SDL_GL_CreateContext(window);
  SDL_SetRelativeMouseMode(SDL_TRUE);
  SDL_GL_SetSwapInterval(1);

  // Initialize GLEW
  if (glewInit() != GLEW_OK) {
    printf("Failed to initialize GLEW\n");
    return -1;
  }

  //load image
  int width, height, channels;

  unsigned char *data = stbi_load("heightmap.png", &width, &height, &channels, 0);

  if (data == NULL) {
    printf("Failed to load image\n");
    return -1;
  }

  printf("%d, %d\n", width, height);

  Vertex **map;

  map = malloc(sizeof(Vertex*)*width);

  for (int i = 0; i < width; i++) {
    map[i] = malloc(sizeof(Vertex)*height);
  }

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      int index = (i + j * width) * channels;
      unsigned char pixelValue = data[index];

      float heightValue = pixelValue / 255.0f;

      map[i][j].x = i;
      map[i][j].y = heightValue * 250;
      map[i][j].z = j;
    }
  }

  Vertex *vertices = malloc(sizeof(Vertex) * width * height);

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      vertices[i * height + j] = map[i][j];
    }
  }

  glViewport(0, 0, VIEWWIDTH, VIEWHEIGHT);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);

  unsigned int EBO;
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * width * height, vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);  

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
    glDrawArrays(GL_POINTS, 0, width*height);
    glBindVertexArray(0);

    SDL_GL_SwapWindow(window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);
  glDeleteProgram(shaderProgram);

  stbi_image_free(data);

  SDL_GL_DeleteContext(glContext);
  SDL_DestroyWindow(window);
  SDL_Quit();
  return 0;
}
