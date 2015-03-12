#include "GUI.h"
#include "../Run/Interface.h"
#include <stdio.h>
#include "../IO/Font.h"
#include "../List.h"
#include "../IO/LoadGUI.h"

extern char KEY, CAPSLOCK;

#if defined(WIN32)
#include <windows.h>
#endif

void button_draw(Button *button, int id, GLfloat text_pos_x, GLfloat text_pos_y)
{
    glBindTexture(GL_TEXTURE_2D, button[id].texture);
    draw_box(button[id].x1 ,button[id].y1 ,button[id].x2 ,button[id].y2);
    glColor3f(button[id].text_color_r, button[id].text_color_g, button[id].text_color_b);
    print(button[id].name, button[id].fnt, text_pos_x, text_pos_y);
}

GLfloat get_length_String(struct Nodes *begin, const struct Nodes *end, Font *fnt)
{
    GLfloat length=0;
    struct Nodes *i=begin;

    while(i!=end)
    {
        length+=fnt->symbol[(unsigned char)i->data][2] - fnt->symbol[(unsigned char)i->data][0];
        i=i->next;
    }

    if(end)
        length+=fnt->symbol[(unsigned char)end->data][2] - fnt->symbol[(unsigned char)end->data][0];

    return length;
}

GLfloat get_width_string(const char *text, Font *fnt)
{
    int i;
    GLfloat length_symbol_x, size_text=0;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    for(i=0; text[i]; i++)
    {
        length_symbol_x=fnt->symbol[(unsigned char)text[i]][2] - fnt->symbol[(unsigned char)text[i]][0];
        size_text+=length_symbol_x;
    }

    return size_text;
}

void print(const char *text, Font *fnt, GLfloat x, GLfloat y)
{
    int i;
    GLfloat length_symbol_x, length_symbol_y, xt;

    xt=x;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    for(i=0; text[i]; i++)
    {
        if(text[i]==10)
        {
            i++;
            x=xt;
            y-=length_symbol_y;
        }

        length_symbol_y = fnt->symbol[(unsigned char)text[i]][3] - fnt->symbol[(unsigned char)text[i]][1];
        length_symbol_x = fnt->symbol[(unsigned char)text[i]][2] - fnt->symbol[(unsigned char)text[i]][0];

        glBegin(GL_QUADS);
            glTexCoord2f(fnt->symbol[(unsigned char)text[i]][0], fnt->symbol[(unsigned char)text[i]][1]); glVertex2f(x, y);
            glTexCoord2f(fnt->symbol[(unsigned char)text[i]][0], fnt->symbol[(unsigned char)text[i]][3]); glVertex2f(x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)text[i]][2], fnt->symbol[(unsigned char)text[i]][3]); glVertex2f(x+length_symbol_x, y+length_symbol_y);
            glTexCoord2f(fnt->symbol[(unsigned char)text[i]][2], fnt->symbol[(unsigned char)text[i]][1]); glVertex2f(x+length_symbol_x, y);
        glEnd();

        x+=length_symbol_x;
    }
}

void print_list(List *text, int size_string, Font *fnt, GLfloat x, GLfloat y)
{
    int i;
    struct Node *tmp=text->begin;
    GLfloat length_symbol_x, length_symbol_y, xt;

    xt=x;

    glBindTexture(GL_TEXTURE_2D, fnt->texture_id);

    for(i=0; i<size_string; i++)
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

void mover(GLfloat *x1, GLfloat *y1, GLfloat *x2, GLfloat *y2, int id)
{
    static int press_id=-1;
    static GLfloat press_x, press_y;

    if(key(VK_LBUTTON))
    {
        if(press_id==-1)
        {
            press_x=mouse_coord_x;
            press_y=mouse_coord_y;

            if((*x1<mouse_coord_x && *x2>mouse_coord_x && *y1<mouse_coord_y && *y2>mouse_coord_y))
            {
                press_id=id;

                *x1+=mouse_coord_x-press_x;
                *x2+=mouse_coord_x-press_x;
                *y1+=mouse_coord_y-press_y;
                *y2+=mouse_coord_y-press_y;
            }
        }
        else if(press_id==id)
        {
            *x1+=mouse_coord_x-press_x;
            *x2+=mouse_coord_x-press_x;
            *y1+=mouse_coord_y-press_y;
            *y2+=mouse_coord_y-press_y;

            press_x=mouse_coord_x;
            press_y=mouse_coord_y;
        }
    }
    else
    {
        press_id=-1;
    }
}

char button(Button *button, int size, int id)
{
    int i;
    char down=0;
    GLfloat width_text=get_width_string(button[id].name, button[id].fnt),
            text_pos_x=(button[id].x2-button[id].x1-width_text)/2+button[id].x1,
            text_pos_y=(button[id].y2-button[id].y1-(button[id].fnt->symbol[40][3] - button[id].fnt->symbol[40][1]))/2+button[id].y1;

    if(mouse_coord_x>=button[id].x1 && mouse_coord_x<=button[id].x2 && mouse_coord_y>=button[id].y1 && mouse_coord_y<=button[id].y2)
    {
        //mouse_coord_x=-1;
        //mouse_coord_y=-1;

        if(key(VK_LBUTTON))
        {
            if(button[id].flash>0.7) button[id].flash-=ms;
            glColor3f(button[id].flash, button[id].flash, button[id].flash);
            button_draw(button, id, text_pos_x, text_pos_y);
            down=0;
            button[id].press=1;
        }
        else
        {
            if(button[id].flash<1) button[id].flash+=ms;
            glColor3f(button[id].flash, button[id].flash, button[id].flash);
            button_draw(button, id, text_pos_x, text_pos_y);
            down=1;
        }

        if(button[id].press==1 && down==1)
        {
            for(i=0; i<size; i++) button[i].press=0;
            return 1;
        }

        return 0;
    }
    else
    {
        button[id].press=0;

        if(button[id].flash>0.8) button[id].flash-=ms;
        if(button[id].flash<0.8) button[id].flash+=ms;
        glColor3f(button[id].flash, button[id].flash, button[id].flash);
        button_draw(button, id, text_pos_x, text_pos_y);
    }

    return 0;
}

struct Nodes *end_of_render_string(struct Nodes *begin, GLfloat length, Font *fnt)
{
    struct Nodes *end=begin;
    GLfloat cur_length=0;

    while(end->next)
    {
        cur_length+=fnt->symbol[(int)end->data][2]-fnt->symbol[(int)end->data][0];
        if(cur_length>length)
            return end->previous;
    }

    return end;
}

void recalc_begin(TextLine *tl)
{
    GLfloat length=tl->x2-tl->x1;

    if(length<0)
        length*=-1;

    tl->begin=tl->end;
    if(tl->begin==0)
        tl->begin=tl->text->begin;

    while(tl->begin)
    {
        length-=tl->fnt->symbol[(int)tl->begin->data][2]-tl->fnt->symbol[(int)tl->begin->data][0];
        if(length<0)
            break;
        tl->begin=tl->begin->previous;
    }

    if(tl->begin)
        tl->begin=tl->begin->next;
}

void recalc_end(TextLine *tl)
{
    GLfloat length=tl->x2-tl->x1;

    if(length<0)
        length*=-1;

    tl->end=tl->begin;
    if(tl->end==0)
        tl->end=tl->text->begin;

    while(tl->end)
    {
        length-=tl->fnt->symbol[(int)tl->end->data][2] - tl->fnt->symbol[(int)tl->end->data][0];
        if(length<0)
            break;
        tl->end=tl->end->next;
    }

    if(tl->end)
        tl->end=tl->end->previous;
}

void text_line(TextLine *text_line, int size, int id)
{
    TextLine *tl=text_line+id;
    GLfloat text_height=tl->fnt->symbol[40][3]-tl->fnt->symbol[40][1];
    struct Nodes *i;
    char k=0;
    GLfloat x,y, length;

    length=tl->x2-tl->x1;
    if(length<0)
        length*=-1;

    glBindTexture(GL_TEXTURE_2D, tl->texture);
    if(tl->activity)
        glColor3f(1,1,1);
    else glColor3f(0.8, 0.8, 0.8);
    draw_box(tl->x1, tl->y, tl->x2, tl->y+text_height);

    if(key_up(VK_LBUTTON))
    {
        if(tl->x1<mouse_coord_x && tl->x2>mouse_coord_x && tl->y<mouse_coord_y && tl->y+text_height>mouse_coord_y)
            tl->activity=GL_TRUE;
        else tl->activity=GL_FALSE;
    }

    if(tl->activity)
    {
        k=KEY;

        if(key_up(VK_CAPITAL))
            CAPSLOCK=!CAPSLOCK;

        if(k==VK_RIGHT)
        {
            if(tl->cur)
            {
                if(tl->cur->next)
                {
                    if(tl->cur==tl->end)
                    {
                        tl->end=tl->cur->next;
                        recalc_begin(tl);
                    }
                    tl->cur=tl->cur->next;
                }
            }
            else
                tl->cur=tl->text->begin;
        }
        else if(k==VK_LEFT)
        {
            if(tl->text->begin && tl->cur)
            {
                if(tl->begin==tl->cur)
                {
                    tl->begin=tl->cur->previous;
                    recalc_end(tl);
                }
                tl->cur=tl->cur->previous;
            }
        }
        else if(k==VK_BACK && tl->cur)
        {
            i=tl->cur;

            if(i->next)
                i->next->previous=i->previous;
            if(i->previous)
                i->previous->next=i->next;
            else
                tl->text->begin=i->next;

            tl->cur=i->previous;

            if(i==tl->begin)
                tl->begin=tl->begin->previous;
            recalc_end(tl);

            free(i);
        }
        else if(k==VK_DELETE)
        {
            if(tl->cur)
            {
                if(tl->cur->next)
                {
                    i=tl->cur->next;

                    if(i->next)
                        i->next->previous=i->previous;
                    if(i->previous)
                        i->previous->next=i->next;

                    free(i);
                }
            }
            else
            {
                if(tl->text->begin)
                {
                    i=tl->text->begin;

                    if(tl->text->begin->next)
                        tl->text->begin->next->previous=0;
                    tl->text->begin=tl->text->begin->next;

                    free(i);
                }
            }

            recalc_end(tl);
        }
        else if((k<='Z' && k >=' '))
        {
            if(k<='Z' && k>='A')
            {
                if(CAPSLOCK)
                {
                    if(key(VK_SHIFT))
                        k+='a'-'A';
                }
                else if(!key(VK_SHIFT))
                    k+='a'-'A';
            }

            if(tl->cur)
            {
                i=malloc(sizeof(struct Nodes));
                i->next=tl->cur->next;
                i->previous=tl->cur;
                i->data=k;

                if(tl->cur->next)
                    tl->cur->next->previous=i;
                tl->cur->next=i;

                tl->cur=i;
            }
            else
            {
                i=malloc(sizeof(struct Nodes));
                i->next=tl->text->begin;
                i->previous=0;
                i->data=k;

                tl->cur=i;
                tl->text->begin=i;
            }

            recalc_end(tl);
            if(tl->end)
            {
                tl->end=tl->end->next;
                if(tl->end==tl->cur)
                    recalc_begin(tl);
                else recalc_end(tl);
            }
        }
        //--------------DRAW CURSOR--------------------
        if(tl->cur)
        {
            if(tl->begin)
                x=get_length_String(tl->begin, tl->cur, tl->fnt)+tl->x1;
            else
                x=get_length_String(tl->text->begin, tl->cur, tl->fnt)+tl->x1;
        }
        else
            x=tl->x1;
        y=tl->y;

        glDisable(GL_TEXTURE_2D);
        glColor3f(tl->cur_color_r+tl->cur_color_flash, tl->cur_color_g+tl->cur_color_flash, tl->cur_color_b+tl->cur_color_flash);
        glBegin(GL_LINES);
            glVertex2f(x,y);
            glVertex2f(x,y+tl->fnt->symbol[40][3]-tl->fnt->symbol[40][1]);
        glEnd();
        glEnable(GL_TEXTURE_2D);

        if(tl->cur_color_zatuhanie)
            tl->cur_color_flash-=2*ms;
        else
            tl->cur_color_flash+=2*ms;

        if(tl->cur_color_flash+tl->cur_color_b>1 || tl->cur_color_flash+tl->cur_color_g>1 || tl->cur_color_flash+tl->cur_color_r>1)
            tl->cur_color_zatuhanie=1;
        if(tl->cur_color_flash<0)
            tl->cur_color_zatuhanie=0;
    }

    glColor3f(tl->text_color_r, tl->text_color_g, tl->text_color_b);

    if(tl->begin)
        str_teil_print(tl->begin, tl->end, tl->fnt, tl->x1, tl->y);
    else
        str_teil_print(tl->text->begin, tl->end, tl->fnt, tl->x1, tl->y);
}

GLclampf list_box_vertical(ListBox *lb, int size, int id)
{
    static GLboolean verify=GL_TRUE;
    static int this=-1;
    GLfloat delta;
    GLclampf percent;

    if(this==id || (mouse_coord_x>lb[id].x1 && mouse_coord_x<lb[id].x2 && mouse_coord_y>lb[id].y1+lb[id].cur_pos && mouse_coord_y<lb[id].y1+lb[id].cur_pos+lb[id].button_height))
    {
        if(key(VK_LBUTTON))
        {
            lb[id].activity=GL_TRUE;

            this=id;
            if(verify==GL_TRUE)
            {
                lb[id].temp_dy=mouse_coord_y;
                verify=GL_FALSE;
            }

            delta=mouse_coord_y-lb[id].temp_dy;
            lb[id].cur_pos+=delta;

            if(lb[id].cur_pos < 0)
                lb[id].cur_pos=0;
            else if(lb[id].cur_pos > lb[id].y2-lb[id].y1-lb[id].button_height)
                lb[id].cur_pos=lb[id].y2-lb[id].y1-lb[id].button_height;

            lb[id].temp_dy=mouse_coord_y;
        }
        else
        {
            lb[id].activity=GL_FALSE;
            this=-1;
            verify=GL_TRUE;
        }
    }
    else
    {
        verify=GL_TRUE;
    }

    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, lb[id].texture_fon);

    draw_box(lb[id].x1, lb[id].y1, lb[id].x2, lb[id].y2);
    glBindTexture(GL_TEXTURE_2D, lb[id].texture_button);
    if(lb[id].activity==GL_TRUE)
        glColor3f(1,1,1);
    else glColor3f(0.7, 0.7, 0.7);
    draw_box(lb[id].x1, lb[id].y1+lb[id].cur_pos, lb[id].x2, lb[id].y1+lb[id].cur_pos+lb[id].button_height);

    percent=lb[id].cur_pos/(lb[id].y2-lb[id].y1-lb[id].button_height);

    return percent;
}

GLclampf list_box_horizontal(ListBox *lb, int size, int id)
{
    static GLboolean verify=GL_TRUE;
    static int this=-1;
    GLfloat delta;
    GLclampf percent;

    if(this==-1)
    {
        if(key(VK_LBUTTON) && mouse_coord_x>lb[id].x1+lb[id].cur_pos && mouse_coord_x<lb[id].x1+lb[id].cur_pos+lb[id].button_height && mouse_coord_y>lb[id].y1 && mouse_coord_y<lb[id].y2)
        {
            this=id;
            lb[id].activity=GL_TRUE;
            lb[id].temp_dy=mouse_coord_x;
        }
        else
            lb[id].activity=GL_FALSE;
    }
    else if(this==id)
    {
        if(key(VK_LBUTTON))
        {
            delta=mouse_coord_x-lb[id].temp_dy;
            lb[id].cur_pos+=delta;

            if(lb[id].cur_pos < 0)
                lb[id].cur_pos=0;
            else if(lb[id].cur_pos > lb[id].x2-lb[id].x1-lb[id].button_height)
                lb[id].cur_pos=lb[id].x2-lb[id].x1-lb[id].button_height;

            lb[id].temp_dy=mouse_coord_x;
        }
        else
        {
            this=-1;
        }
    }

    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D, lb[id].texture_fon);

    draw_box(lb[id].x1, lb[id].y1, lb[id].x2, lb[id].y2);
    glBindTexture(GL_TEXTURE_2D, lb[id].texture_button);
    if(lb[id].activity==GL_TRUE)
        glColor3f(1,1,1);
    else glColor3f(0.7, 0.7, 0.7);
    draw_box(lb[id].x1+lb[id].cur_pos, lb[id].y1, lb[id].x1+lb[id].button_height+lb[id].cur_pos, lb[id].y2);

    percent=lb[id].cur_pos/(lb[id].x2-lb[id].x1-lb[id].button_height);

    return percent;
}

void build_GUI()
{

}

void draw_GUI(GUI *data)
{
    int i;

    for(i=0; i<data->sz_buttons; i++)
    {
        button(data->button.but, data->sz_buttons, i);
    }
    for(i=0; i<data->sz_list_box; i++)
    {
        list_box_horizontal(data->listbox.lb, data->sz_list_box, i);
    }
    for(i=0; i<data->sz_text_lines; i++)
    {
        text_line(data->tl.tl, data->sz_text_lines, i);
    }
}
