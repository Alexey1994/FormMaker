#include <stdlib.h>
#include "TextEditor.h"
#include "GUI.h"

void text_init(Text *txt, int width, int height)
{
    txt=malloc(sizeof(Text));
    insert_list(&(txt->data), '\n', 0);
    txt->max_height=height;
    txt->max_width=width;
    txt->length_symbols=0;
    txt->length_current_pos_to_symbol=1;
    txt->cur_x=0;
    txt->cur_y=0;
    txt->ins_x=0;
    txt->ins_y=0;
    txt->max_x=0;
    txt->max_y=0;
}

void text_set_position(Text *txt, int x, int y)
{
    txt->cur_x=x;
    txt->cur_y=y;
    txt->length_current_pos_to_symbol=text_get_length(txt);
}

void text_down(Text *txt)
{
    printf("%d", txt->cur_y);
    if(txt->cur_y==txt->max_y-1) {printf("h");return;}
    txt->cur_y++;
    txt->length_current_pos_to_symbol=text_get_length(txt);
}

void text_up(Text *txt)
{
    if(txt->cur_y==0) return;
    txt->cur_y--;
    txt->length_current_pos_to_symbol=text_get_length(txt);
}

void text_right(Text *txt)
{
    if(txt->cur_x==txt->max_width)
    {
        txt->cur_x=0;
        text_down(txt);
    }
    else
    {
        txt->cur_x++;
        txt->length_current_pos_to_symbol++;
    }
}

void text_left(Text *txt)
{
    if(txt->cur_x==0)
        text_up(txt);
    else
    {
        txt->cur_x--;
        txt->length_current_pos_to_symbol--;
    }
}

void text_reset_insert(Text *txt)
{
    txt->ins_x=0;
    txt->ins_y=0;
}

int text_get_length(Text *txt)
{
    struct Node *tmp=txt->data.begin;
    int i=0, y=0, lx=0;

    while(y<txt->cur_y)
    {
        if(tmp->data=='\n')
        {
            y++;
            lx++;
            i+=lx;
            lx=0;
        }

        lx++;

        if(lx>txt->max_x)
        {
            y++;
            i+=lx;
            lx=0;
        }

        tmp=tmp->next;
    }

    i+=txt->cur_x;

    return i;
}

void text_insert(Text *txt, char symbol)
{
    txt->length_symbols++;

    if(symbol=='\n' || txt->cur_x==txt->max_width)
    {
        if(txt->cur_y==txt->max_y)
            txt->cur_x=0;
        txt->max_y++;
    }
    else txt->max_x++;

    insert_list(&txt->data, symbol, txt->length_current_pos_to_symbol);
    txt->length_current_pos_to_symbol++;
    txt->cur_x++;
    if(txt->cur_x==txt->max_width)
    {
        txt->max_y++;
        txt->cur_x=0;
        txt->cur_y++;
    }
}
