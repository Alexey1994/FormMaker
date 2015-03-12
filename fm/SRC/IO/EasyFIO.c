#include "EasyFIO.h"
#include "../Error/Error.h"
#include "../Math.h"
#include <stdlib.h>

char *get_name(FILE *f, char *ch)
{
    int k=0;
    char *s=malloc(sizeof(char)*256);

    while(1)
    {
        if(feof(f) || *ch=='\n') break;
        s[k]=*ch;
        k++;
        *ch=fgetc(f);
    }
    s[k]='\0';

    return s;
}

char *get_string(FILE *f, char *ch)
{
    int k=0;
    char *s=malloc(sizeof(char)*256);

    while(1)
    {
        if(feof(f) || *ch==' ' || *ch==13) break;
        s[k]=*ch;
        k++;
        *ch=fgetc(f);
    }
    s[k]='\0';
    if(*ch==13) *ch=fgetc(f);

    return s;
}

void skip(FILE *f, char *ch)
{
    while(*ch==' ' || *ch==13) *ch=fgetc(f);
    if(*ch==13) *ch=fgetc(f);
}

void skip_string(FILE *f, char *ch)
{
    while(*ch!='\n' && !feof(f)) *ch=fgetc(f);
    if(*ch=='\n') *ch=fgetc(f);
}

void get_value_GLuint(FILE *f, char *ch, GLuint *value)
{
    char *s;

    skip(f, ch);
    s=get_string(f, ch);
    *value=char_to_GLuint(s);
    free(s);
}

void get_value_GLfloat(FILE *f, char *ch, GLfloat *value)
{
    char *s;

    skip(f, ch);
    s=get_string(f, ch);
    *value=char_to_GLfloat(s);
    free(s);
}

void copy_file_to_file(FILE *f, const char *name)
{
    char ch;
    FILE *t=fopen(name, "rb");

    if(t==0)
    {
        file_not_found(name);
        return;
    }

    while(fread(&ch, 1, 1, t))
        fwrite(&ch, 1, 1, f);

    fclose(t);
}
