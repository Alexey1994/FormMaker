#include "LoadGUI.h"
#include <stdio.h>
#include <stdlib.h>

char *to_char(String *s)
{
    struct Nodes *i=s->begin;
    char *arr=malloc(str_length(s)+1);
    int c=0;

    arr[0]=0;

    while(i)
    {
        arr[c]=i->data;
        c++;
        i=i->next;
    }

    arr[c]='\0';

    return arr;
}

GUI* load_GUI(const char *name)
{
    GUI *data=malloc(sizeof(GUI));
    FILE *f=fopen(name, "rb");
    char id[]="GUI", tmp_id[4], tmp, ch, *c;
    int i, j;
    unsigned int tex_id, num;
    String *s=str_init("");

    if(f==0)
    {
        str_free(s);
        return 0;
    }

    tmp_id[3]='\0';
    fread(tmp_id, 1, 3, f);

    if(string_comparision(id, tmp_id)==0)
    {
        str_free(s);
        return 0;
    }

    fread(&data->sz_fonts, sizeof(unsigned int), 1, f);
    fread(&data->sz_textures, sizeof(unsigned int), 1, f);
    fread(&data->sz_buttons, sizeof(unsigned int), 1, f);
    fread(&data->sz_list_box, sizeof(unsigned int), 1, f);
    fread(&data->sz_text_lines, sizeof(unsigned int), 1, f);

    data->fnt=malloc(sizeof(Font)*data->sz_fonts);
    data->texture_id=malloc(sizeof(unsigned int)*data->sz_textures);

    data->button.but=malloc(sizeof(Button)*data->sz_buttons);
    data->button.index_fonts=malloc(sizeof(unsigned int)*data->sz_buttons);
    data->button.index_textures=malloc(sizeof(unsigned int)*data->sz_buttons);

    data->listbox.lb=malloc(sizeof(ListBox)*data->sz_list_box);
    data->listbox.index_fonts=malloc(sizeof(unsigned int)*data->sz_list_box);
    data->listbox.index_textures1=malloc(sizeof(unsigned int)*data->sz_list_box);
    data->listbox.index_textures2=malloc(sizeof(unsigned int)*data->sz_list_box);

    data->tl.tl=malloc(sizeof(TextLine)*data->sz_text_lines);
    data->tl.index_fonts=malloc(sizeof(unsigned int)*data->sz_text_lines);
    data->tl.index_textures=malloc(sizeof(unsigned int)*data->sz_text_lines);

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
    return data;
}

void save_GUI(GUI *data, const char *name, const char **name_fonts, const char **name_texture_ids)
{
    FILE *f=fopen(name, "wb");
    char id[]="GUI";
    int i;
    unsigned int tmp=0;

    fwrite(id, 3, 1, f);

    fwrite(&data->sz_fonts, sizeof(int), 1, f);
    fwrite(&data->sz_textures, sizeof(int), 1, f);
    fwrite(&data->sz_buttons, sizeof(int), 1, f);
    fwrite(&data->sz_list_box, sizeof(int), 1, f);
    fwrite(&data->sz_text_lines, sizeof(int), 1, f);

    for(i=0; i<data->sz_fonts; i++)
    {
        fwrite(name_fonts[i], string_length(name_fonts[i]), 1, f);
        fputc('\0', f);
    }
    for(i=0; i<data->sz_textures; i++)
    {
        fwrite(name_texture_ids[i], string_length(name_texture_ids[i]), 1, f);
        fputc('\0', f);
    }

    for(i=0; i<data->sz_buttons; i++)
    {
        fwrite(data->button.index_fonts+i, sizeof(unsigned int), 1, f);

        fwrite(data->button.but[i].name, string_length(data->button.but[i].name), 1, f);
        fputc('\0', f);

        fwrite(data->button.index_textures+i, sizeof(unsigned int), 1, f);

        fwrite(&data->button.but[i].text_color_b, sizeof(GLfloat), 1, f);
        fwrite(&data->button.but[i].text_color_g, sizeof(GLfloat), 1, f);
        fwrite(&data->button.but[i].text_color_r, sizeof(GLfloat), 1, f);

        fwrite(&data->button.but[i].x1, sizeof(GLfloat), 1, f);
        fwrite(&data->button.but[i].x2, sizeof(GLfloat), 1, f);
        fwrite(&data->button.but[i].y1, sizeof(GLfloat), 1, f);
        fwrite(&data->button.but[i].y2, sizeof(GLfloat), 1, f);
    }

    for(i=0; i<data->sz_list_box; i++)
    {
        fwrite(data->listbox.index_textures1+i, sizeof(unsigned int), 1, f);
        fwrite(data->listbox.index_textures2+i, sizeof(unsigned int), 1, f);

        fwrite(&data->listbox.lb[i].button_height, sizeof(GLfloat), 1, f);

        fwrite(&data->listbox.lb[i].x1, sizeof(GLfloat), 1, f);
        fwrite(&data->listbox.lb[i].x2, sizeof(GLfloat), 1, f);
        fwrite(&data->listbox.lb[i].y1, sizeof(GLfloat), 1, f);
        fwrite(&data->listbox.lb[i].y2, sizeof(GLfloat), 1, f);
    }

    for(i=0; i<data->sz_text_lines; i++)
    {
        fwrite(data->tl.index_fonts+i, sizeof(unsigned int), 1, f);
        fwrite(data->tl.index_textures+i, sizeof(unsigned int), 1, f);

        fwrite(&data->tl.tl[i].activity, 1, 1, f);

        fwrite(&data->tl.tl[i].cur_color_b, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].cur_color_g, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].cur_color_r, sizeof(GLfloat), 1, f);

        fwrite(&data->tl.tl[i].text_color_b, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].text_color_g, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].text_color_r, sizeof(GLfloat), 1, f);

        fwrite(&data->tl.tl[i].max_length, sizeof(GLuint), 1, f);

        fwrite(&data->tl.tl[i].cur_color_flash, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].x1, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].x2, sizeof(GLfloat), 1, f);
        fwrite(&data->tl.tl[i].y, sizeof(GLfloat), 1, f);
    }

    fclose(f);
}

void free_GUI(GUI *gui)
{
    int i;

    for(i=0; i<gui->sz_buttons; i++)
    {

    }
}
