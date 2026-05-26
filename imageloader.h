#ifndef IMAGE_LOADER_H_INCLUDED
#define IMAGE_LOADER_H_INCLUDED

#include <glut.h>

unsigned char* loadBMP(
    const char* filename,
    int& width,
    int& height);

GLuint textureFromBMP(const char* filename);

#endif