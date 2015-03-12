#include "Render/Render.h"
#include "List.h"
#include "IO/Font.h"
#include "IO/CFG.h"
#include "Error/Error.h"
#include "Sound/WAV.h"
#include <GL/glext.h>
#include "glext.h"
#include "IO/OBJ.h"
#include "Render/Render.h"
#include <GL/glu.h>
#include "Run/Interface.h"
#include "IO/Texture.h"
#include "shader.h"
#include <stdlib.h>
#include <windows.h>
#include "Video/AVI.h"
#include "IO/DAE.h"
#include "manipulator.h"
#include <math.h>
#include "string.h"
#include "IO/LoadGUI.h"

extern GLboolean bQuit;
extern GLfloat ms;
extern GLfloat aspect, mouse_coord_x, mouse_coord_y;
extern GLfloat x,y,z;

static Font *font;

void load_GUI_to_buffer(const char *name, GUI *data)
{
    FILE *f=fopen(name, "rb");
    char id[]="GUI", tmp_id[4], tmp, ch, *c;
    int i, j;
    unsigned int tex_id, num;
    String *s=str_init("");

    if(f==0)
    {
        str_free(s);
        return;
    }

    tmp_id[3]='\0';
    fread(tmp_id, 1, 3, f);

    if(string_comparision(id, tmp_id)==0)
    {
        str_free(s);
        return;
    }

    fread(&data->sz_fonts, sizeof(unsigned int), 1, f);
    fread(&data->sz_textures, sizeof(unsigned int), 1, f);
    fread(&data->sz_buttons, sizeof(unsigned int), 1, f);
    fread(&data->sz_list_box, sizeof(unsigned int), 1, f);
    fread(&data->sz_text_lines, sizeof(unsigned int), 1, f);

    for(i=0; i<data->sz_fonts; i++)
    {
        ch=fgetc(f);
        while(ch)
        {
            str_push(s, ch);
            ch=fgetc(f);
        }
        c=to_char(s);
        data->fnt[i]=load_font(c);
        free(c);
        str_clear(s);
    }

    for(i=0; i<data->sz_textures; i++)
    {
        ch=fgetc(f);
        while(ch)
        {
            str_push(s, ch);
            ch=fgetc(f);
        }
        c=to_char(s);
        data->texture_id[i]=load_texture(c);
        free(c);
        str_clear(s);
    }

    for(i=0; i<data->sz_buttons; i++)
    {
        data->button.but[i].flash=0.8;
        fread(&num, sizeof(unsigned int), 1, f);
        data->button.but[i].fnt=data->fnt[num];
        data->button.index_fonts[i]=num;

        ch=fgetc(f);
        while(ch)
        {
            str_push(s, ch);
            ch=fgetc(f);
        }
        data->button.but[i].name=to_char(s);
        str_clear(s);

        data->button.but[i].press=0;

        fread(&num, sizeof(unsigned int), 1, f);
        data->button.but[i].texture=data->texture_id[num];
        data->button.index_textures[i]=num;

        fread(&data->button.but[i].text_color_b, sizeof(GLfloat), 1, f);
        fread(&data->button.but[i].text_color_g, sizeof(GLfloat), 1, f);
        fread(&data->button.but[i].text_color_r, sizeof(GLfloat), 1, f);

        fread(&data->button.but[i].x1, sizeof(GLfloat), 1, f);
        fread(&data->button.but[i].x2, sizeof(GLfloat), 1, f);
        fread(&data->button.but[i].y1, sizeof(GLfloat), 1, f);
        fread(&data->button.but[i].y2, sizeof(GLfloat), 1, f);
    }

    for(i=0; i<data->sz_list_box; i++)
    {
        data->listbox.lb[i].activity=0;
        data->listbox.lb[i].button_height=0;
        data->listbox.lb[i].cur_pos=0;
        data->listbox.lb[i].temp_dy=0;

        fread(&num, sizeof(unsigned int), 1, f);
        data->listbox.lb[i].texture_button=data->texture_id[num];
        data->listbox.index_textures1[i]=num;

        fread(&num, sizeof(unsigned int), 1, f);
        data->listbox.lb[i].texture_fon=data->texture_id[num];
        data->listbox.index_textures2[i]=num;

        fread(&data->listbox.lb[i].button_height, sizeof(GLfloat), 1, f);

        fread(&data->listbox.lb[i].x1, sizeof(GLfloat), 1, f);
        fread(&data->listbox.lb[i].x2, sizeof(GLfloat), 1, f);
        fread(&data->listbox.lb[i].y1, sizeof(GLfloat), 1, f);
        fread(&data->listbox.lb[i].y2, sizeof(GLfloat), 1, f);
    }

    for(i=0; i<data->sz_text_lines; i++)
    {
        fread(&num, sizeof(unsigned int), 1, f);
        data->tl.tl[i].fnt=data->fnt[num];
        data->tl.index_fonts[i]=num;

        fread(&num, sizeof(unsigned int), 1, f);
        data->tl.tl[i].texture=data->texture_id[num];
        data->tl.index_textures[i]=num;

        fread(&data->tl.tl[i].activity, 1, 1, f);

        fread(&data->tl.tl[i].cur_color_b, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].cur_color_g, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].cur_color_r, sizeof(GLfloat), 1, f);

        fread(&data->tl.tl[i].text_color_b, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].text_color_g, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].text_color_r, sizeof(GLfloat), 1, f);

        fread(&data->tl.tl[i].max_length, sizeof(GLuint), 1, f);

        fread(&data->tl.tl[i].cur_color_flash, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].x1, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].x2, sizeof(GLfloat), 1, f);
        fread(&data->tl.tl[i].y, sizeof(GLfloat), 1, f);

        data->tl.tl[i].text=str_init("");
        data->tl.tl[i].cur=data->tl.tl[i].text->begin;
        data->tl.tl[i].begin=0;
        data->tl.tl[i].end=data->tl.tl[i].cur;
    }

    fclose(f);
    str_free(s);
}

void add_button(GUI *gui)
{
    int i=gui->sz_buttons;

    gui->sz_buttons++;

    gui->button.but[i].flash=0.8;
    gui->button.but[i].fnt=font;
    gui->button.but[i].name="0";
    gui->button.but[i].press=0;
    gui->button.but[i].texture=1;
    gui->button.but[i].text_color_b=0;
    gui->button.but[i].text_color_g=0;
    gui->button.but[i].text_color_r=0;
    gui->button.but[i].x1=0;
    gui->button.but[i].x2=0.5;
    gui->button.but[i].y1=0;
    gui->button.but[i].y2=0.3;

    gui->button.index_fonts[i]=0;
    gui->button.index_textures[i]=0;
}

void add_list_box(GUI *gui)
{
    int i=gui->sz_list_box;

    gui->sz_list_box++;

    gui->listbox.lb[i].activity=0;
    gui->listbox.lb[i].button_height=0.2;
    gui->listbox.lb[i].cur_pos=0;
    gui->listbox.lb[i].temp_dy=0;
    gui->listbox.lb[i].texture_button=1;
    gui->listbox.lb[i].texture_fon=3;
    gui->listbox.lb[i].x1=0;
    gui->listbox.lb[i].x2=0.5;
    gui->listbox.lb[i].y1=0;
    gui->listbox.lb[i].y2=0.1;

    gui->listbox.index_textures1[i]=1;
    gui->listbox.index_textures2[i]=2;
    gui->listbox.index_fonts[i]=0;
}

void add_text_line(GUI *gui)
{
    int i=gui->sz_text_lines;

    gui->sz_text_lines++;

    gui->tl.tl[i].activity=1;
    gui->tl.tl[i].text=str_init("");
    gui->tl.tl[i].cur=gui->tl.tl[i].text->begin;
    gui->tl.tl[i].begin=0;
    gui->tl.tl[i].end=gui->tl.tl[i].cur;

    gui->tl.tl[i].cur_color_b=0;
    gui->tl.tl[i].cur_color_g=0;
    gui->tl.tl[i].cur_color_r=0;

    gui->tl.tl[i].cur_color_flash=0;
    gui->tl.tl[i].cur_color_zatuhanie=0;

    gui->tl.tl[i].fnt=font;
    gui->tl.tl[i].max_length=0;
    gui->tl.tl[i].texture=1;

    gui->tl.tl[i].text_color_b=0;
    gui->tl.tl[i].text_color_g=0;
    gui->tl.tl[i].text_color_r=0;

    gui->tl.tl[i].x1=0;
    gui->tl.tl[i].x2=0.3;
    gui->tl.tl[i].y=0.3;

    gui->tl.index_fonts[i]=0;
    gui->tl.index_textures[i]=0;
}

void draw()
{
    static unsigned int sz_fonts=10,
                        sz_textures=1000,
                        sz_buttons=1000,
                        sz_list_box=1000,
                        sz_text_lines=1000;

    static char verify=1;
    static Button buttons[20];
    static GLuint texture[40];
    static TextLine tl;
    static ListBox lb[3];
    static GUI *gui;
    static unsigned int current;
    int i, j, k;

    static char *fn[1]={"Fonts/build.fnt"}, *tx[3]={"Button/button.tga", "ListBox/button.tga", "ListBox/Fon.tga"};

    if(verify)
    {
        texture[0]=load_texture("Button/button.tga");
        texture[1]=load_texture("TextLine/fon.tga");
        texture[2]=load_texture("ListBox/fon.tga");
        texture[3]=load_texture("ListBox/button.tga");
        texture[4]=load_texture("Button/exit.tga");

        font=load_font("Fonts/build.fnt");

        gui=(GUI*)malloc(sizeof(GUI));
        gui->fnt=malloc(sizeof(Font)*sz_fonts);
        gui->texture_id=malloc(sizeof(unsigned int)*sz_textures);

        gui->button.but=malloc(sizeof(Button)*sz_buttons);
        gui->button.index_fonts=malloc(sizeof(unsigned int)*sz_buttons);
        gui->button.index_textures=malloc(sizeof(unsigned int)*sz_buttons);

        gui->listbox.lb=malloc(sizeof(ListBox)*sz_list_box);
        gui->listbox.index_fonts=malloc(sizeof(unsigned int)*sz_list_box);
        gui->listbox.index_textures1=malloc(sizeof(unsigned int)*sz_list_box);
        gui->listbox.index_textures2=malloc(sizeof(unsigned int)*sz_list_box);

        gui->tl.tl=malloc(sizeof(TextLine)*sz_text_lines);
        gui->tl.index_fonts=malloc(sizeof(unsigned int)*sz_text_lines);
        gui->tl.index_textures=malloc(sizeof(unsigned int)*sz_text_lines);

        load_GUI_to_buffer("interface.gui", gui);
/*
        gui->sz_textures=3;
        gui->sz_fonts=1;

        gui->sz_text_lines=0;
        gui->sz_buttons=0;
        gui->sz_list_box=0;*/


        buttons[0].press=0;
        buttons[0].fnt=font;
        buttons[0].name="новая кнопка";
        buttons[0].texture=texture[0];
        buttons[0].x1=-aspect;
        buttons[0].x2=buttons[0].x1+0.5;
        buttons[0].y1=0.9;
        buttons[0].y2=1;
        buttons[0].text_color_r=0.0;
        buttons[0].text_color_g=0.0;
        buttons[0].text_color_b=0.0;
        buttons[0].flash=0.8;

        buttons[1].press=0;
        buttons[1].fnt=font;
        buttons[1].name="новая линия прокрутки";
        buttons[1].texture=texture[0];
        buttons[1].x1=-aspect;
        buttons[1].x2=buttons[1].x1+0.7;
        buttons[1].y1=0.8;
        buttons[1].y2=0.9;
        buttons[1].text_color_r=0.0;
        buttons[1].text_color_g=0.0;
        buttons[1].text_color_b=0.0;
        buttons[1].flash=0.8;

        buttons[2].press=0;
        buttons[2].fnt=font;
        buttons[2].name="очистить";
        buttons[2].texture=texture[0];
        buttons[2].x1=-aspect;
        buttons[2].x2=buttons[2].x1+0.7;
        buttons[2].y1=0.7;
        buttons[2].y2=0.8;
        buttons[2].text_color_r=0.0;
        buttons[2].text_color_g=0.0;
        buttons[2].text_color_b=0.0;
        buttons[2].flash=0.8;

        buttons[3].press=0;
        buttons[3].fnt=font;
        buttons[3].name="новая полоса набора";
        buttons[3].texture=texture[0];
        buttons[3].x1=-aspect;
        buttons[3].x2=buttons[2].x1+0.7;
        buttons[3].y1=0.6;
        buttons[3].y2=0.7;
        buttons[3].text_color_r=0.0;
        buttons[3].text_color_g=0.0;
        buttons[3].text_color_b=0.0;
        buttons[3].flash=0.8;

        lb[0].activity=1;
        lb[0].button_height=0.1;
        lb[0].cur_pos=0;
        lb[0].temp_dy=0;
        lb[0].texture_button=texture[0];
        lb[0].texture_fon=texture[2];
        lb[0].x1=0;
        lb[0].x2=0.5;
        lb[0].y1=-1;
        lb[0].y2=-0.9;

        lb[1].activity=0;
        lb[1].button_height=0.1;
        lb[1].cur_pos=0;
        lb[1].temp_dy=0;
        lb[1].texture_button=texture[0];
        lb[1].texture_fon=texture[2];
        lb[1].x1=0;
        lb[1].x2=0.5;
        lb[1].y1=-0.9;
        lb[1].y2=-0.8;

        lb[2].activity=0;
        lb[2].button_height=0.1;
        lb[2].cur_pos=0;
        lb[2].temp_dy=0;
        lb[2].texture_button=texture[0];
        lb[2].texture_fon=texture[2];
        lb[2].x1=0;
        lb[2].x2=0.5;
        lb[2].y1=-0.8;
        lb[2].y2=-0.7;

        tl.activity=1;
        tl.text=str_init("");
        tl.begin=tl.text->begin;
        tl.end=tl.text->begin;
        tl.cur=tl.text->begin;
        tl.max_length=0;
        tl.fnt=font;
        tl.text_color_b=0;
        tl.text_color_g=0.3;
        tl.text_color_r=0;
        tl.cur_color_b=0.5;
        tl.cur_color_g=0;
        tl.cur_color_r=0.5;
        tl.cur_color_flash=0;
        tl.cur_color_zatuhanie=0;
        tl.x1=-0.5;
        tl.x2=0.1;
        tl.y=0.3;

        verify=0;
    }

    if(key_up(VK_ESCAPE)) bQuit=1;

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glPushMatrix();

    glColor3f(1,0,0);
    print("Демо", font, -aspect,0.92);
    glColor3f(0,0,0);

    //text_line(&tl, 1, 0);

    if(button(buttons, 4, 0))
    {
        current=gui->sz_buttons;
        add_button(gui);
    }
    if(button(buttons, 4, 1))
    {
        current=gui->sz_list_box;
        add_list_box(gui);
    }
    if(button(buttons, 4, 2))
    {
        gui->sz_list_box=0;
        gui->sz_buttons=0;
        gui->sz_text_lines=0;
    }
    if(button(buttons, 4, 3))
    {
        current=gui->sz_text_lines;
        add_text_line(gui);
    }

    for(i=0; i<gui->sz_buttons; i++)
    {
        mover(&gui->button.but[i].x1,
              &gui->button.but[i].y1,
              &gui->button.but[i].x2,
              &gui->button.but[i].y2,
              i);
    }
    for(j=gui->sz_buttons; j<gui->sz_buttons+gui->sz_list_box; j++)
    {
        mover(&gui->listbox.lb[j-gui->sz_buttons].x1,
              &gui->listbox.lb[j-gui->sz_buttons].y1,
              &gui->listbox.lb[j-gui->sz_buttons].x2,
              &gui->listbox.lb[j-gui->sz_buttons].y2,
              j);
    }
    for(k=gui->sz_buttons+gui->sz_list_box; k<gui->sz_buttons+gui->sz_list_box+gui->sz_text_lines; k++)
    {
        GLfloat y2=gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].y +
                   gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].fnt->symbol[40][3] -
                   gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].fnt->symbol[40][1];

        mover(&gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].x1,
              &gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].y,
              &gui->tl.tl[k-gui->sz_buttons-gui->sz_list_box].x2,
              &y2,
              k);
    }

    if(key_up('S'))
    {
        save_GUI(gui, "interface.gui", fn, tx);
    }

    draw_GUI(gui);
    tl.text_color_b=list_box_horizontal(lb, 3, 0);
    tl.text_color_g=list_box_horizontal(lb, 3, 1);
    tl.text_color_r=list_box_horizontal(lb, 3, 2);
}

void draw_mouse(GLfloat mouse_coord_x, GLfloat mouse_coord_y, Model *mdl)
{
    glTranslatef(mouse_coord_x, mouse_coord_y ,0);
    glDisable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    draw_obj(mdl);
    glEnable(GL_TEXTURE_2D);
}
