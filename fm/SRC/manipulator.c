#include "manipulator.h"
#include <GL/glu.h>
#include <math.h>
#include "Run/Interface.h"
#include "Render/Render.h"

extern GLfloat mouse_coord_x, mouse_coord_y, ms;
extern GLfloat x,y,z;

GLfloat SPEED_LEFT=1,
        SPEED_RIGHT=1,
        SPEED_UP=1,
        SPEED_DOWN=1;

void actor()
{
    static GLfloat teta=0, phi=0;

    if(key('W'))
    {
        x+=SPEED_UP*sin(-teta*M_PI/180.0f);
        z+=SPEED_UP*cos(-teta*M_PI/180.0f);
    }
    if(key('S'))
    {
        x-=SPEED_DOWN*sin(-teta*M_PI/180.0f);
        z-=SPEED_DOWN*cos(-teta*M_PI/180.0f);
    }
    if(key('A'))
    {
        x+=SPEED_LEFT*cos(teta*M_PI/180.0f);
        z+=SPEED_LEFT*sin(teta*M_PI/180.0f);
    }
    if(key('D'))
    {
        x-=SPEED_RIGHT*cos(teta*M_PI/180.0f);
        z-=SPEED_RIGHT*sin(teta*M_PI/180.0f);
    }

    teta+=10*mouse_coord_x;
    if(phi-10*mouse_coord_y > -90 && phi-10*mouse_coord_y < 90)
        phi-=10*mouse_coord_y;
    glRotatef(phi, 1,0,0);
    glRotatef(teta, 0,1,0);
    glTranslatef(x, y ,z);
}

void kachka()
{
    static float x=0,y=0,z=0, dx=0, dy=0, rx=0, ry=0;

    dx-=mouse_coord_x; dy-=mouse_coord_y;
    rx-=10*dx; ry+=10*dy;
    glRotatef(rx, 0,1,0);
    glRotatef(ry, 1,0,0);

    dx=mouse_coord_x; dy=mouse_coord_y;
}

void draw_object(Object *obj)
{
    glPushMatrix();
    glTranslatef(obj->x, obj->y, obj->z);
    draw_obj(obj->mdl);
    glPopMatrix();
}

void gravity(Object *obj, int sz)
{
    int i;

    for(i=0; i<sz; i++)
    {
        if(obj[i].y>-10)
        {
            obj[i].y-=20*obj[i].t*obj[i].t;
            obj[i].t+=ms;
        }
        else obj[i].t=0;
        draw_object(obj+i);
    }
}
