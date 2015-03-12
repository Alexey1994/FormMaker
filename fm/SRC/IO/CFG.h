#ifndef CFG_H_INCLUDED
#define CFG_H_INCLUDED

#include <GL/gl.h>

typedef struct
{
    GLuint width,
           height,
           fps;
}Config;

Config *load_CFG(const char *name);

#endif // CFG_H_INCLUDED
