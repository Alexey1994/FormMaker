#ifndef LOADGUI_H_INCLUDED
#define LOADGUI_H_INCLUDED

#include <GL/gl.h>
#include "../GUI/GUI.h"

typedef struct
{
    ListBox *lb;
    unsigned int *index_fonts,
                 *index_textures1,
                 *index_textures2;
}LB;

typedef struct
{
    Button *but;
    unsigned int *index_fonts,
                 *index_textures;
}But;

typedef struct
{
    TextLine *tl;
    unsigned int *index_fonts,
                 *index_textures;
}TL;

typedef struct
{
    unsigned int sz_fonts,
                 sz_textures,
                 sz_buttons,
                 sz_list_box,
                 sz_text_lines;

    Font **fnt;
    unsigned int *texture_id;

    LB listbox;
    But button;
    TL tl;

}GUI;

GUI *load_GUI(const char *name);
void save_GUI(GUI *data, const char *name, const char **name_fonts, const char **name_texture_ids);
void draw_GUI(GUI *data);
void free_GUI(GUI *gui);
#endif
