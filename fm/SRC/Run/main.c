#include "InitWindow.h"
#include "../IO/CFG.h"
#include <stdio.h>

GLboolean bQuit=GL_FALSE;
GLfloat aspect, mouse_coord_x, mouse_coord_y;
GLfloat ms=0;
GLfloat x=0,y=0,z=0;
char KEY=0, CAPSLOCK=0;

int main(int argc, char **argv)
{
    Config *cfg=load_CFG("config.cfg");
    ms=1.0f/((GLfloat)cfg->fps+12);

    init_and_run_window(cfg);

    return 0;
}
