#include "../include/3d-engine.h"

GLuint createEBO() {
  GLuint EBO;
  glGenBuffers(1, &EBO);
  return EBO;
}

void bindEBO(GLuint EBO, int size, int *indices) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * size, indices, GL_STATIC_DRAW);
}
