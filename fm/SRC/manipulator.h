#ifndef MANIPU_H_INCLUDED
#define MANIPU_H_INCLUDED

#include <GL/gl.h>
#include "IO/OBJ.h"

typedef struct
{
    GLfloat x,y,z,t;
    Model *mdl;
}Object;

void actor();
void kachka();
void gravity(Object *obj, int sz);

#endif
