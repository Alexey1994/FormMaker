#include "OBJ.h"
#include "EasyFIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "../Math.h"
#include "Texture.h"

Model* load_OBJ(const char *name)
{
    FILE *f=fopen(name, "rb");
    Model *model;
    char ch[1], usemtl[6]={0}, *material, mtllib[6]={0};
    GLuint l_faces=0, l_verteces=0, l_objects=0;
    GLuint i=0, j=0, k=0;
    Face *faces;
    Point *verteces;
    Material *mtl;

    if(f==0) return 0;
    model=malloc(sizeof(Model));

    *ch=fgetc(f);
    while(1)
    {
        if(feof(f)) break;

        if(*ch=='v')
        {
            *ch=fgetc(f);
            if(*ch==' ') l_verteces++;
        }
        else
        if(*ch=='f') l_faces++;
        else
        if(*ch=='o') l_objects++;
        skip_string(f, ch);
    }

    faces=malloc(sizeof(Face)*l_faces);
    verteces=malloc(sizeof(Point)*l_verteces);
    model->name=malloc(sizeof(char)*l_objects);
    model->object_indexes=malloc(sizeof(GLuint)*l_objects);

    rewind(f);

    *ch=fgetc(f);
    while(1)
    {
        if(feof(f)) break;

        if(*ch=='f')
        {
            *ch=fgetc(f);

            get_value_GLuint(f, ch, faces[i]);
            if(*ch!='\n')
                get_value_GLuint(f, ch, faces[i]+1);
            else faces[i][1]=0;
            if(*ch!='\n')
                get_value_GLuint(f, ch, faces[i]+2);
            else faces[i][2]=0;
            if(*ch!='\n')
                get_value_GLuint(f, ch, faces[i]+3);
            else faces[i][3]=0;

            //printf("%d %d %d %d\n", *faces[i], *(faces[i]+1), *(faces[i]+2), *(faces[i]+3));

            i++;
        }
        else
        if(*ch=='v')
        {
            *ch=fgetc(f);

            get_value_GLfloat(f, ch, verteces[j]);
            get_value_GLfloat(f, ch, verteces[j]+1);
            get_value_GLfloat(f, ch, verteces[j]+2);
            //printf("%f %f %f\n", verteces[j][0], verteces[j][1], verteces[j][2]);

            j++;
        }
        else
        if(*ch=='o')
        {
            *ch=fgetc(f);

            skip(f, ch);
            model->name[k]=get_name(f, ch);
            model->object_indexes[k]=i;
            k++;
        }
        else
        if(*ch=='u')
        {
            fgets(usemtl, 6, f);
            *ch=fgetc(f);
            if(string_comparision(usemtl, "semtl"))
            {
                skip(f, ch);
                material=get_string(f, ch);
                //printf(material);
                free(material);
            }
        }
        else
        if(*ch=='m')
        {
            fgets(mtllib, 6, f);
            *ch=fgetc(f);
            if(string_comparision(mtllib, "tllib"))
            {
                skip(f, ch);
                material=get_string(f, ch);
                mtl=load_MTL(material, l_objects+1);
                free(material);
            }
        }

        if(*ch!='\n')
            skip_string(f, ch);
        else *ch=fgetc(f);
    }
    fclose(f);

    model->length_faces=l_faces;
    model->length_verteces=l_verteces;
    model->length_objects=l_objects;
    model->length_normals=0;
    model->faces=faces;
    model->verteces=verteces;
    model->mtl=mtl;

    return model;
}

Model* load_OBJ_calculate_normals_flat(const char *name)
{
    Model *mdl=load_OBJ(name);

    if(mdl==0)
        return 0;

    mdl->normals=malloc(sizeof(Vector)*mdl->length_faces);
    mdl->length_normals=mdl->length_faces;
    calculate_normals_flat(mdl->verteces, mdl->faces, mdl->normals, mdl->length_faces);

    return mdl;
}

Material *load_MTL(const char *name, int size)
{
    FILE *f=fopen(name, "rb");
    if(f==0) return 0;
    size=1;
    Material *mtl=malloc(sizeof(Material)*size);
    int i;
    char ch[1], map_Kd[6], *s;

    *ch=fgetc(f);
    while(1)
    {
        if(feof(f)) break;

        if(*ch=='#') skip_string(f, ch);
        else if(*ch=='K')
        {
            *ch=fgetc(f);
            if(*ch=='a')
            {
                *ch=fgetc(f);
                get_value_GLfloat(f, ch, mtl->ambient);
                get_value_GLfloat(f, ch, mtl->ambient+1);
                get_value_GLfloat(f, ch, mtl->ambient+2);
                mtl->ambient[3]=1.0;

            }
            else if(*ch=='d')
            {
                *ch=fgetc(f);
                get_value_GLfloat(f, ch, mtl->diffuse);
                get_value_GLfloat(f, ch, mtl->diffuse+1);
                get_value_GLfloat(f, ch, mtl->diffuse+2);
                mtl->diffuse[3]=1.0;
            }
            else if(*ch=='s')
            {
                *ch=fgetc(f);
                get_value_GLfloat(f, ch, mtl->specular);
                get_value_GLfloat(f, ch, mtl->specular+1);
                get_value_GLfloat(f, ch, mtl->specular+2);
                mtl->specular[3]=1.0;
            }
        }
        else if(*ch=='m')
        {
            fgets(map_Kd, 6, f);
            *ch=fgetc(f);
            if(string_comparision(map_Kd, "ap_Kd"))
            {
                skip(f, ch);
                s=get_string(f, ch);

                glEnable(GL_TEXTURE_GEN_S);
                glEnable(GL_TEXTURE_GEN_T);
                glEnable(GL_TEXTURE_GEN_R);
                glEnable(GL_TEXTURE_GEN_Q);

                mtl->texture_id=load_texture(s);
                free(s);

                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
                glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

                glDisable(GL_TEXTURE_GEN_S);
                glDisable(GL_TEXTURE_GEN_T);
                glDisable(GL_TEXTURE_GEN_R);
                glDisable(GL_TEXTURE_GEN_Q);
            }
        }

        if(*ch!='\n')
            skip_string(f, ch);
        else *ch=fgetc(f);
    }

    fclose(f);
    free(s);
    return mtl;
}

void free_OBJ(Model *model)
{
    GLuint i;

    free(model->faces);
    free(model->verteces);
    free(model->normals);
    free(model->object_indexes);
    for(i=0; i<model->length_objects; i++)
        free(*(i+model->name));
    glDeleteTextures(1, &model->mtl->texture_id);
    free(model->mtl);
    free(model->name);
    free(model);
}
