#include "Render.h"
#include <GL/gl.h>

void draw_box(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2)
{
    glBegin(GL_QUADS);
        glTexCoord2f(0,0); glVertex2f(x1,y1);
        glTexCoord2f(0,1); glVertex2f(x1,y2);
        glTexCoord2f(1,1); glVertex2f(x2,y2);
        glTexCoord2f(1,0); glVertex2f(x2,y1);
    glEnd();
}

void draw_obj(const Model *obj)
{
    int i, length=obj->length_faces;
    Vector *normals=obj->normals;
    Point *verteces=obj->verteces;
    Face *faces=obj->faces;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, obj->mtl->ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, obj->mtl->diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, obj->mtl->specular);
    //printf("%f %f %f\n", obj->mtl->ambient[0], obj->mtl->ambient[1], obj->mtl->ambient[2]);
    glBindTexture(GL_TEXTURE_2D, obj->mtl->texture_id);
    glEnable(GL_TEXTURE_GEN_S);
    glEnable(GL_TEXTURE_GEN_T);
glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

    glBegin(GL_QUADS);
    for(i=0; i<length; i++)
    {
        /*if(faces[i][1]==0)
        {
            glBegin(GL_POINTS);
                glVertex3fv(verteces[faces[i][0]-1]);
            glEnd();
        }
        else if(faces[i][2]==0)
        {
            glBegin(GL_LINES);
                glVertex3fv(verteces[faces[i][0]-1]);
                glVertex3fv(verteces[faces[i][1]-1]);
            glEnd();glEnable(GL_LIGHT0);
        }*/
        if(faces[i][3]==0)
        {
            glNormal3fv(normals[i]);

            glEnd();
            glBegin(GL_TRIANGLES);
                glTexCoord2f(0,0); glVertex3fv(verteces[faces[i][0]-1]);
                glTexCoord2f(0,1); glVertex3fv(verteces[faces[i][1]-1]);
                glTexCoord2f(1,1); glVertex3fv(verteces[faces[i][2]-1]);
            glEnd();
            glBegin(GL_QUADS);
        }
        else
        {
            glNormal3fv(normals[i]);

            glTexCoord2f(0,0); glVertex3fv(verteces[faces[i][0]-1]);
            glTexCoord2f(1,0); glVertex3fv(verteces[faces[i][1]-1]);
            glTexCoord2f(1,1); glVertex3fv(verteces[faces[i][2]-1]);
            glTexCoord2f(0,1); glVertex3fv(verteces[faces[i][3]-1]);
        }
    }
    glEnd();

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
}
