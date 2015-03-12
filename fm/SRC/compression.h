#ifndef COMPRESSION_H_INCLUDED
#define COMPRESSION_H_INCLUDED

#include <GL/gl.h>
#include <stdio.h>

void RLE(const GLubyte *data, int size_of, int size_data, FILE *f);

#endif // COMPRESSION_H_INCLUDED
