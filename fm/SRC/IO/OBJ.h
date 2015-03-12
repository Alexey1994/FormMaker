#ifndef OBJ_H_INCLUDED
#define OBJ_H_INCLUDED

#include <GL/gl.h>
#include <GL/glext.h>
#include "../Math.h"

typedef struct
{
    GLuint texture_id;
    GLfloat specular[4];
    GLfloat ambient[4];
    GLfloat diffuse[4];
}Material;

typedef struct
{
    Material *mtl;
    char **name;
}OBJMaterial;

typedef struct
{
    char **name;

    GLuint length_faces,
           length_verteces,
           length_normals,
           length_objects,

           *object_indexes;     /*начиная с этого номера строится фейс объекта с номером элемента массива индексов*/

    Point  *verteces;
    Vector *normals;
    Face   *faces;

    Material *mtl;
}Model;

Model* load_OBJ(const char *name);
Model* load_OBJ_calculate_normals_flat(const char *name);
void free_OBJ(Model *model);
Material *load_MTL(const char *name, int size);

#endif // OBJ_H_INCLUDED
