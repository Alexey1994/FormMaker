#ifndef STRINSSS_H_INCLUDED
#define STRINSSS_H_INCLUDED

#include <GL/gl.h>
#include "IO/Font.h"

struct Nodes
{
    char data;
    struct Nodes *next, *previous;
};

typedef struct
{
    struct Nodes *begin;
}String;

int str_length(String *s);

void str_concat(String *s1, String *s2);
void str_push(String *s, char data);
void str_copy(String *str1, String *str2);
void str_clear(String *s);
void str_print(String *s, Font *fnt, GLfloat x, GLfloat y);
void str_teil_print(struct Nodes *begin, struct Nodes *end, Font *fnt, GLfloat x, GLfloat y);
//void text_line_print(struct Nodes *begin, Font *fnt, GLfloat x, GLfloat x2, GLfloat y);
String *str_init(const char *c);
void str_free(String *s);

#endif // STRINSSS_H_INCLUDED
