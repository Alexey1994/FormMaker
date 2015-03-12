#include "Texture.h"
#include "Image.h"
#include <GL/glext.h>

GLuint load_texture(const char *name)
{
    GLuint texture_id;

    FILE *f=fopen(name, "rb");
    texture_id=texture_loader(f);

    return texture_id;
}

GLuint texture_loader(FILE *f)
{
    GLint out_type;
    Image *image=image_loader(f);
    GLuint texture_id;

    if(image==0) return 0;
    if(image->bpp==GL_RGB) out_type=GL_BGR;
    if(image->bpp==GL_RGBA) out_type=GL_BGRA;

    glGenTextures(1, &texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_LINEAR_MIPMAP_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, image->bpp, image->width, image->height, 0, out_type, GL_UNSIGNED_BYTE, (image->data));

    free_image(image);
    return texture_id;
}
