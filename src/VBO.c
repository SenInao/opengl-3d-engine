#include "../include/3d-engine.h"

GLuint createVBO() {
  GLuint VBO;
  glGenBuffers(1, &VBO);
  return VBO;
}

void bindVBO(GLuint VBO, int size, Vertex *vertices) {
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * size, vertices, GL_STATIC_DRAW);
}
