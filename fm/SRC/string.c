#include "string.h"
#include <stdlib.h>

void str_push(String *s, char data)
{
    struct Nodes *count=s->begin, *alloc;

    if(count==0)
    {
        count=malloc(sizeof(struct Nodes));
        count->data=data;
        count->next=0;
        count->previous=0;
        s->begin=count;

        return;
    }

    while(count->next)
        count=count->next;

    alloc=malloc(sizeof(struct Nodes));
    alloc->data=data;
    alloc->next=0;
    alloc->previous=count;

    count->next=alloc;
}

String *str_init(const char *c)
{
    int i=1;
    String *s=malloc(sizeof(String));
    struct Nodes *count;

    s->begin=0;

    if(c[0])
    {
        count=malloc(sizeof(struct Nodes));
        count->data=c[0];
        count->next=0;
        count->previous=0;
        s->begin=count;
    }
    else return s;

    while(c[i])
    {
        count->next=malloc(sizeof(struct Nodes));
        count->next->data=c[i];
        count->next->previous=count;
        count=count->next;

        i++;
    }

    if(s->begin)
        count->next=0;

    return s;
}

void str_clear(String *s)
{
    struct Nodes *count=s->begin, *tmp;

    while(count)
    {
        tmp=count->next;
        free(count);
        count=tmp;
    }

    s->begin=0;
}

void str_copy(String *str1, String *str2)
{
    struct Nodes *count=str2->begin, *i=0;

    if(str1==str2)
        return;
    str_clear(str1);

    if(count)
    {
        i=malloc(sizeof(struct Nodes));
        i->data=count->data;
        i->next=0;
        i->previous=0;
        str1->begin=i;
        count=count->next;
    }

    while(count)
    {
        i->next=malloc(sizeof(struct Nodes));
        i->next->previous=i;
        i->next->data=count->data;
        i=i->next;

        count=count->next;
    }

    if(i)
        i->next=0;
}

void str_concat(String *s1, String *s2)
{
    struct Nodes *i=s1->begin, *count=s2->begin;
    int j, k;

    if(s1!=s2)
    {
        if(i)
        {
            while(i->next)
                i=i->next;
        }
        else
            str_copy(s1, s2);

        while(count)
        {
            i->next=malloc(sizeof(struct Nodes));
            i->next->previous=i;
            i->next->data=count->data;
            i=i->next;

            count=count->next;
        }

        if(i)
            i->next=0;
    }
    else if(i)
    {
        k=0;
        while(i->next)
        {
            i=i->next;
            k++;
        }

        for(j=0; j<=k; j++)
        {
            i->next=malloc(sizeof(struct Nodes));
            i->next->previous=i;
            i->next->data=count->data;
            i=i->next;

            count=count->next;
        }

        i->next=0;
    }
}

int str_length(String *s)
{
    int i=0;
    struct Nodes *count=s->begin;

    while(count)
    {
        count=count->next;
        i++;
    }

    return i;
}

void str_print(String *s, Font *fnt, GLfloat x, GLfloat y)
{
    int i;
    struct Nodes *tmp=s->begin;
    GLfloat length_symbol_x, length_symbol_y, xt;

    xt=x;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    while(tmp)
    {
        if(tmp->data==10)
        {
            i++;
            x=xt;
            y-=length_symbol_y;
        }

        length_symbol_y = fnt->symbol[(unsigned char)tmp->data][3] - fnt->symbol[(unsigned char)tmp->data][1];
        length_symbol_x = fnt->symbol[(unsigned char)tmp->data][2] - fnt->symbol[(unsigned char)tmp->data][0];

        glBegin(GL_QUADS);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x, y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x+length_symbol_x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x+length_symbol_x, y);
        glEnd();

        x+=length_symbol_x;
        tmp=tmp->next;
    }
}

void str_teil_print(struct Nodes *begin, struct Nodes *end, Font *fnt, GLfloat x, GLfloat y)
{
    int i;
    struct Nodes *tmp=begin;
    GLfloat length_symbol_x, length_symbol_y, xt;

    xt=x;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    while(tmp!=end)
    {
        if(tmp->data==10)
        {
            i++;
            x=xt;
            y-=length_symbol_y;
        }

        length_symbol_y = fnt->symbol[(unsigned char)tmp->data][3] - fnt->symbol[(unsigned char)tmp->data][1];
        length_symbol_x = fnt->symbol[(unsigned char)tmp->data][2] - fnt->symbol[(unsigned char)tmp->data][0];

        glBegin(GL_QUADS);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x, y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x+length_symbol_x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x+length_symbol_x, y);
        glEnd();

        x+=length_symbol_x;
        tmp=tmp->next;
    }

    if(end)
    {
        length_symbol_y = fnt->symbol[(unsigned char)end->data][3] - fnt->symbol[(unsigned char)end->data][1];
        length_symbol_x = fnt->symbol[(unsigned char)end->data][2] - fnt->symbol[(unsigned char)end->data][0];

        glBegin(GL_QUADS);
            glTexCoord2f(fnt->symbol[(unsigned char)end->data][0], fnt->symbol[(unsigned char)end->data][1]); glVertex2f(x, y);
            glTexCoord2f(fnt->symbol[(unsigned char)end->data][0], fnt->symbol[(unsigned char)end->data][3]); glVertex2f(x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)end->data][2], fnt->symbol[(unsigned char)end->data][3]); glVertex2f(x+length_symbol_x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)end->data][2], fnt->symbol[(unsigned char)end->data][1]); glVertex2f(x+length_symbol_x, y);
        glEnd();
    }
}

void text_line_print(struct Nodes *begin, Font *fnt, GLfloat x, GLfloat x2, GLfloat y)
{
    int i;
    struct Nodes *tmp=begin;
    GLfloat length_symbol_x, length_symbol_y, xt, length;

    length=x2-x;
    if(length<0)
        length*=-1;
    xt=x;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    while(length>x && tmp)
    {
        if(tmp->data==10)
        {
            i++;
            x=xt;
            y-=length_symbol_y;
        }

        length_symbol_y = fnt->symbol[(unsigned char)tmp->data][3] - fnt->symbol[(unsigned char)tmp->data][1];
        length_symbol_x = fnt->symbol[(unsigned char)tmp->data][2] - fnt->symbol[(unsigned char)tmp->data][0];

        glBegin(GL_QUADS);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x, y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][0], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][3]); glVertex2f(x+length_symbol_x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)tmp->data][2], fnt->symbol[(unsigned char)tmp->data][1]); glVertex2f(x+length_symbol_x, y);
        glEnd();

        x+=length_symbol_x;
        tmp=tmp->next;
    }
}

void str_free(String *s)
{
    struct Nodes *count=s->begin, *tmp;

    while(count)
    {
        tmp=count->next;
        free(count);
        count=tmp;
    }

    free(s);
}
