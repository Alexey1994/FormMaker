#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED

#include "../Math.h"
#include "Image.h"
#include <stdio.h>

typedef struct
{
    Vector4 symbol[256];
    GLuint texture_id;
    char *name;
}Font;

Font *font_loader(FILE *f);
Font *load_font(const char *name);
void free_font(Font *font);
void save_font(Font *font, Image *image);
void make_font(const char *tga_name, const char *font_name);

#endif
