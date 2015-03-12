#ifndef TEXTEDITOR_H_INCLUDED
#define TEXTEDITOR_H_INCLUDED

#include "../List.h"

typedef struct
{
    List data;
    int length_current_pos_to_symbol,
        length_symbols,
        cur_x,
        cur_y,
        ins_x,
        ins_y,
        max_x,
        max_y,
        max_height, //-1 -без ограничений
        max_width;
}Text;

void text_init(Text *txt, int width, int height);
void text_set_position(Text *txt, int x, int y);
void text_down(Text *txt);
void text_up(Text *txt);
void text_left(Text *txt);
void text_right(Text *txt);
void text_reset_insert(Text *txt);
void text_insert(Text *txt, char symbol);
int text_get_length(Text *txt);

void text_delete(Text *txt);
//void text_delete(Text *txt, int begin, int end);

#endif // TEXTEDITOR_H_INCLUDED
