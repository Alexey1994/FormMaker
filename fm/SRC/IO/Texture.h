#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <GL/gl.h>
#include <stdio.h>

GLuint load_texture(const char *name);
GLuint texture_loader(FILE *f);

#endif // TEXTURE_H_INCLUDED
