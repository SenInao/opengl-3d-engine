#include "../include/3d-engine.h"
#include <stb_image.h>

Model createHeightmapModel(char *filename) {
  Model heightmap;

  int width, height, channels;

  unsigned char *data = stbi_load("heightmap.png", &width, &height, &channels, 0);

  if (data == NULL) {
    printf("Failed to load image\n");
  }

  printf("%d, %d\n", width, height);

  Vertex **map;
  Vertex *vertices;

  map = malloc(sizeof(Vertex*)*width);
  vertices = malloc(sizeof(Vertex) * width * height);

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

      vertices[i * height + j] = map[i][j];
    }
  }

  int *indices = malloc(sizeof(int) * 6 * (width-1) * (height-1));

  for (int i = 0; i < width-1; i++) {
    for (int j = 0; j < height-1; j++) {
      int topLeft = i * height + j;
      int topRight = i * height + j+1;
      int bottomLeft = (i+1) * height + j;
      int bottomRight = (i+1) * height + j+1;

      int listPos = (i*(height-1)+j)*6;

      indices[listPos] = topLeft;
      indices[listPos+1] = bottomLeft;
      indices[listPos+2] = topRight;

      indices[listPos+3] = topRight;
      indices[listPos+4] = bottomLeft;
      indices[listPos+5] = bottomRight;
    }
  }

  heightmap.vertices = vertices;
  heightmap.indices = indices;
  heightmap.sizeIndices = 6*(width-1)*(height-1);
  heightmap.sizeVertices = height*width;

  stbi_image_free(data);

  return heightmap;
};
