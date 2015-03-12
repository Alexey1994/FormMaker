#include "Font.h"
#include "Image.h"
#include <stdlib.h>
#include "../Error/Error.h"
#include "Image/ImageType.h"
#include "EasyFIO.h"
#include "Texture.h"
#include <GL/glext.h>

Font *load_font(const char *name)
{
    Font *font;
    FILE *f=fopen(name, "rb");

    if(f==0)
    {
        warning_divider();
        file_not_found(name);
        return 0;
    }
    font=font_loader(f);

    if(font==0)
        contents_error(name);

    fclose(f);
    return font;
}

Font *font_loader(FILE *f)
{
    Font *font;
    char verify[]="FONT", temp[8];
    GLuint i, width, height, size;
    GLubyte *data;

    if(fread(temp, 4, 1, f)==0) return 0;
    if(string_comparision(verify, temp)==0)
    {
        warning("this not font");
        return 0;
    }

    font=(Font*)malloc(sizeof(Font));

    for(i=0; i<256; i++)
    {
        if(fread(font->symbol[i], 1, sizeof(GLfloat), f)==0) {free(font); return 0;}
        if(fread(font->symbol[i]+1, 1, sizeof(GLfloat), f)==0) {free(font); return 0;}
        if(fread(font->symbol[i]+2, 1, sizeof(GLfloat), f)==0) {free(font); return 0;}
        if(fread(font->symbol[i]+3, 1, sizeof(GLfloat), f)==0) {free(font); return 0;}
    }

    if(fread(&width, 1, sizeof(GLuint), f)==0) {free(font); return 0;}
    if(fread(&height, 1, sizeof(GLuint), f)==0) {free(font); return 0;}

    size=width*height*4;
    data=(GLubyte *)malloc(size);

    if(fread(data, size, 1, f)==0) {free(font); free(data); return 0;};

    glGenTextures(1, &font->texture_id);
    glBindTexture(GL_TEXTURE_2D, font->texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_BGRA, GL_UNSIGNED_BYTE, data);

    free(data);
    return font;
}

void make_font(const char *tga_name, const char *font_name)
{
    FILE *f=fopen(font_name, "a");
    copy_file_to_file(f, tga_name);
}

void save_font(Font *font, Image *image)
{
    FILE *f=fopen(font->name, "wb");
    char verify[]="FONT";
    int i, size=image->width*image->height*4;

    if(f==0) return;

    fwrite(verify, 4, 1, f);

    for(i=0; i<256; i++)
    {
        fwrite(font->symbol[i], 1, sizeof(GLfloat), f);
        fwrite(font->symbol[i]+1, 1, sizeof(GLfloat), f);
        fwrite(font->symbol[i]+2, 1, sizeof(GLfloat), f);
        fwrite(font->symbol[i]+3, 1, sizeof(GLfloat), f);
    }

    fwrite(&image->width, 1, sizeof(GLuint), f);
    fwrite(&image->height, 1, sizeof(GLuint), f);
    fwrite(image->data, size, 1, f);

    fclose(f);
}
