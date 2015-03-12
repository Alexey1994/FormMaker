#include "DAE.h"
#include "EasyFIO.h"
#include "../Math.h"
#include <stdio.h>
#include <stdlib.h>

char str[255];

void get_begin(FILE *f, char *ch)
{
    int i=0;

    if(*ch!='<')
    {
        str[0]=0;
        return;
    }

    while(1)
    {
        *ch=fgetc(f);
        if(*ch=='>') {*ch=fgetc(f); break;}
        str[i]=*ch;
        i++;
    }
    str[i]=0;
}

void get_value(FILE *f, char *ch)
{
    int i=0;

    while(1)
    {
        str[i]=*ch;
        *ch=fgetc(f);
        if(*ch=='<') break;
        i++;
    }

    str[i+1]=0;
}

Model *load_DAE(const char *name)
{
    FILE *f=fopen(name, "rb");
    if(f==0) return 0;
    Model *model=(Model*)malloc(sizeof(Model));
    char asset[]="asset",
         asset_end[]="/asset",

         library_cameras[]="library_cameras",
         library_cameras_end[]="/library_cameras",

         library_lights[]="library_lights",
         library_lights_end[]="/library_lights",

         library_effects[]="library_effects",
         library_effects_end[]="/library_effects",

         library_materials[]="library_materials",
         library_materials_end[]="/library_materials",

         library_geometries[]="library_geometries",
         library_geometries_end[]="/library_geometries",

         library_visual_scenes[]="library_visual_scenes",
         library_visual_scenes_end[]="/library_visual_scenes",

         scene[]="scene",
         scene_end[]="/scene",

         extra[]="extra",
         extra_end[]="/extra",

         mesh[]="mesh",
         mesh_end[]="/mesh",

         geometry_id[]="geometry id=";

    char *tmp;

    *tmp=fgetc(f);
    while(1)
    {
        if(feof(f)) break;

        skip(f, tmp);
        get_begin(f, tmp);

        if(string_comparision(asset, str))
        {
            while(string_comparision(asset_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
                /*
                if(feof(f)) break;

                skip(f, tmp);
                get_begin(f, tmp);
                printf(str);

                if(string_comparision(author, str))
                {
                    get_value(f, tmp);
                    string_copy(is_autor, str);
                    printf(is_autor);
                }


                if(str[0]!=0) printf("\n");
                //skip_string(f, tmp);*/
            }
        }
        else
        if(string_comparision(library_cameras, str))
        {
            while(string_comparision(library_cameras_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
        else
        if(string_comparision(library_lights, str))
        {
            while(string_comparision(library_lights_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
        else
        if(string_comparision(library_effects, str))
        {
            while(string_comparision(library_effects_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
        else
        if(string_comparision(library_materials, str))
        {
            while(string_comparision(library_materials_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
        else
        if(string_comparision(library_geometries, str))
        {
            while(string_comparision(library_geometries_end, str)==0)
            {
                get_begin(f, tmp);
                {
                    skip(f, tmp);
                    get_begin(f, tmp);
                    if(string_comparision(mesh, str))
                    {
                        while(string_comparision(mesh_end, str)==0)
                        {
                            skip(f, tmp);
                            get_begin(f, tmp);
                            if(string_comparision(mesh, str))
                            {
                                while(string_comparision(mesh_end, str)==0)
                                {
                                    skip(f, tmp);
                                    get_begin(f, tmp);

                                }
                            }
                        }
                    }
                    else
                    if(string_comparision(extra, str))
                    {
                        while(string_comparision(extra_end, str)==0)
                        {
                            while(*tmp!='<') *tmp=fgetc(f);
                            get_begin(f, tmp);
                        }
                    }
                }
            }
        }
        else
        if(string_comparision(library_visual_scenes, str))
        {
            while(string_comparision(library_visual_scenes_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
        else
        if(string_comparision(scene, str))
        {
            while(string_comparision(scene_end, str)==0)
            {
                while(*tmp!='<') *tmp=fgetc(f);
                get_begin(f, tmp);
            }
        }
    }

    fclose(f);
    return model;
}
