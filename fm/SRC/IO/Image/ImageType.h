#ifndef IMAGETYPE_H_INCLUDED
#define IMAGETYPE_H_INCLUDED

#include <GL/gl.h>
#include "../Image.h"

typedef struct
{
    GLubyte header[6];
    GLuint  image_size,
            height,
            width;
    GLint   bpp;             /*24 или 32*/
}TGA;

typedef struct
{
    //BITMAPFILEHEADER

    GLubyte  type[2];
    GLuint   size;
    GLushort reserved1,
             reserved2;
    GLuint   off_bits;

    //BITMAPINFO

    GLuint   version;
    GLint    width,
             height;

    GLushort planes,
             bpp;

    GLuint   compression,
             size_image,
             x_pels_per_meter,
             y_pels_per_meter,
             clr_used,
             clr_important;

}BMP;

typedef struct
{

}JPEG;

typedef struct
{

}PNG;

Image *load_TGA(const char *name);
Image *TGA_loader(FILE *f);
void save_TGA(FILE *f, const char *compressed, Image *image);

Image *load_BMP(const char *name);
Image *BMP_loader(FILE *f);
void save_BMP(FILE *f);

Image *load_JPEG(const char *name);
Image *JPEG_loader(FILE *f);
void save_JPEG(FILE *f);

Image *load_PNG(const char *name);
Image *PNG_loader(FILE *f);
void save_PNG(FILE *f);

#endif
