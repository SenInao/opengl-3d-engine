#include <GL/glew.h>
#include "../include/3d-engine.h"

GLuint createVAO() {
  GLuint VAO;
  glGenVertexArrays(1, &VAO);
  return VAO;
};

void bindVAO(GLuint VAO) {
  glBindVertexArray(VAO);
}

void enableVertexAttr() {
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
  glEnableVertexAttribArray(0);  
}
