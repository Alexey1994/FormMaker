#ifndef IMAGE_H_INCLUDED
#define IMAGE_H_INCLUDED

#include <GL/gl.h>
#include <stdio.h>

typedef struct
{
    GLuint width,
           height;
    GLint  bpp;         /*RGB или RGBA*/

    GLubyte *data;
}Image;

Image *load_image(const char *name);
void free_image(Image *data);
Image *image_loader(FILE *f);

#endif
