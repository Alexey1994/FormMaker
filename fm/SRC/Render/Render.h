#ifndef RENDER_H_INCLUDED
#define RENDER_H_INCLUDED

#include "../IO/OBJ.h"
#include <GL/gl.h>

typedef GLfloat Quaternion[4];

struct Bone
{
    struct Bone *parent;
    Quaternion orient;
    Vector pos;
};

void draw_obj(const Model *obj);
void draw_box(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

#endif
