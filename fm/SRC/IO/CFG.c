#include <stdio.h>
#include <stdlib.h>
#include "../Error/Error.h"
#include "../Math.h"
#include "EasyFIO.h"
#include "CFG.h"

Config *load_CFG(const char *name)
{
    Config *tmp=malloc(sizeof(Config));
    char ch[1], *s1, *s2;
    FILE *f;
    f=fopen(name, "rb");

    tmp->width=1;
    tmp->height=1;
    tmp->fps=1;

    if(f==0)
    {
        file_not_found(name);
        return tmp;
    }

    while(1)
    {
        *ch=fgetc(f);
        if(feof(f)) break;
        skip(f, ch);

        if(*ch=='#')
            while(*ch!=10)
                *ch=fgetc(f);
        else
        {
            s1=get_string(f, ch);
            skip(f, ch);
            s2=get_string(f, ch);
            if(string_comparision(s1, "width")) tmp->width=char_to_GLuint(s2);
            else if(string_comparision(s1, "height")) tmp->height=char_to_GLuint(s2);
            else if(string_comparision(s1, "fps")) tmp->fps=char_to_GLuint(s2);
            else if(string_comparision(s1, "end")) {free(s1);free(s2);break;}
            free(s1);
            free(s2);
        }
    }

    fclose(f);
    return tmp;
}
