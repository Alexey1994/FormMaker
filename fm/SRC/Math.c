#include "Math.h"
#include <math.h>
#include <stdlib.h>

/*---------------------------math-----------------------------*/

void VectorOrtogonalize(const Vector a, const Vector b, Vector normale)
{
    normale[0]=a[1]*b[2]-a[2]*b[1];
    normale[1]=a[2]*b[0]-a[0]*b[2];
    normale[2]=a[0]*b[1]-a[1]*b[0];
}

void VectorNormalize(Vector a)
{
    GLfloat length=sqrt(a[0]*a[0]+a[1]*a[1]+a[2]*a[2]);
    a[0]*=length;
    a[1]*=length;
    a[2]*=length;
}

/*---------------------------convertors------------------------*/

GLuint char_to_GLuint(const char *ch)
{
    GLuint temp=0;
    int i=0;

    while(1)
    {
        temp+=ch[i]-'0';
        i++;
        if(ch[i]=='\0')
            break;
        temp*=10;
    }

    return temp;
}

GLint char_to_GLint(const char *ch)
{
    GLint temp=0;
    int i;

    if(ch[0]=='-')
    {
        i=1;
        while(1)
        {
            temp+=ch[i]-'0';
            i++;
            if(ch[i]=='\0')
                break;
            temp*=10;
        }

        return -temp;
    }
    else
    {
        i=0;
        while(1)
        {
            temp+=ch[i]-'0';
            i++;
            if(ch[i]=='\0')
                break;
            temp*=10;
        }

        return temp;
    }
}

GLfloat char_to_GLfloat(const char *ch)
{
    GLfloat temp=0, pow=10;
    int i;

    if(ch[0]=='-')
    {
        i=1;

        while(1)
        {
            temp+=ch[i]-'0';
            i++;
            if(ch[i]=='.' || ch[i]=='\0')
                break;
            temp*=10;
        }

        i++;
        while(ch[i])
        {
            temp+=(ch[i]-'0')/pow;
            pow*=10;
            i++;
        }

        return -temp;
    }
    else
    {
        i=0;

        while(1)
        {
            temp+=ch[i]-'0';
            i++;
            if(ch[i]=='.' || ch[i]=='\0')
                break;
            temp*=10;
        }

        i++;
        while(ch[i])
        {
            temp+=(ch[i]-'0')/pow;
            pow*=10;
            i++;
        }

        return temp;
    }
}

/*------------------------------strings--------------------------------*/

void string_copy(char *s1, const char *s2)
{
    int i=0;

    while(s2[i])
    {
        s1[i]=s2[i];
        i++;
    }
    s1[i]='\0';
}

GLubyte string_comparision(const char *s_1, const char *s_2)
{
    unsigned int i=0;

    while(s_1[i])
    {
        if(s_1[i]!=s_2[i])
            return 0;
        i++;
    }

    return 1;
}

GLuint string_length(const char *s)
{
    GLuint tmp=0;
    while(s[tmp]) tmp++;
    return tmp;
}

GLubyte memory_comparision(const GLubyte *m1, const GLubyte *m2, GLuint size)
{
    GLuint i=0;

    for(i=0; i<size; i++)
        if(m1[i]!=m2[i]) return 0;

    return 1;
}

void calculate_normals_flat(Point *verteces, Face *faces, Vector *normals, int length_faces)
{
    Vector a,b,c;
    int i;

    for(i=0; i<length_faces; i++)
    {
        a[0]=verteces[faces[i][2]-1][0] - verteces[faces[i][0]-1][0];
        a[1]=verteces[faces[i][2]-1][1] - verteces[faces[i][0]-1][1];
        a[2]=verteces[faces[i][2]-1][2] - verteces[faces[i][0]-1][2];

        b[0]=verteces[faces[i][1]-1][0] - verteces[faces[i][0]-1][0];
        b[1]=verteces[faces[i][1]-1][1] - verteces[faces[i][0]-1][1];
        b[2]=verteces[faces[i][1]-1][2] - verteces[faces[i][0]-1][2];

        VectorOrtogonalize(a,b,c);
        VectorNormalize(c);
        VectorCopy(normals[i],c);
    }
}
