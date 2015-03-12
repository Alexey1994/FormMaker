#ifndef GUI_H_INCLUDED
#define GUI_H_INCLUDED

#include <GL/gl.h>
#include "../List.h"
#include "../IO/Font.h"
#include "../String.h"

extern GLfloat aspect, mouse_coord_x, mouse_coord_y;
extern GLfloat ms;

typedef struct
{
    char *name;
    Font *fnt;
    GLfloat x1,y1,x2,y2;
    GLfloat text_color_r, text_color_g, text_color_b;
    char press;
    GLuint texture;
    GLfloat flash;
}Button;

typedef struct
{
    String *text;
    struct Nodes *cur, *begin, *end;
    Font *fnt;
    GLfloat x1,y,x2;
    GLfloat text_color_r,
            text_color_g,
            text_color_b,

            cur_color_r,
            cur_color_g,
            cur_color_b,
            cur_color_flash;
    char cur_color_zatuhanie;
    char activity;
    GLuint texture, max_length;
}TextLine;

typedef struct
{
    GLfloat x1,y1,x2,y2;
    GLfloat cur_pos, button_height;
    GLuint texture_fon;
    GLuint texture_button;
    char activity;

    GLfloat temp_dy;
}ListBox;

void print(const char *text, Font *fnt, GLfloat x, GLfloat y);
GLclampf get_width_string(const char *text, Font *fnt);

GLfloat get_length_String(struct Nodes *begin, const struct Nodes *end, Font *fnt);

void text_line(TextLine *text_line, int size, int id);

char button(Button *button, int size, int id);

GLclampf list_box_vertical(ListBox *lb, int size, int id);
GLclampf list_box_horizontal(ListBox *lb, int size, int id);

void draw_box(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);

void mover(GLfloat *x1, GLfloat *y1, GLfloat *x2, GLfloat *y2, int id);

#endif
