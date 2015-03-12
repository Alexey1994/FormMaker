#include "Error.h"
#include <stdio.h>
#include <time.h>
#include "GL/gl.h"

//extern *waring, *error;

char *get_time(struct tm *time)
{
    int i=0;
    static char ch[16];
//day
    if(time->tm_mday>10)
    {
        ch[i]=time->tm_mday/10+'0';
        i++;
        ch[i]=time->tm_mday%10+'0';
    }
    else
        ch[i]=time->tm_mday+'0';
    i++;
    ch[i]='.';
    i++;
//monday
    ch[i]=time->tm_mon/10+'0';
    i++;
    ch[i]=time->tm_mon%10+1+'0';

    i++;
    ch[i]='.';
    i++;
//year
    if(time->tm_year<100)
    {
        ch[i]='1';
        i++;
        ch[i]='9';
    }
    else
    {
        ch[i]='2';
        i++;
        ch[i]='0';
    }
    i++;
    ch[i]=(time->tm_year/10)%10+'0';
    i++;
    ch[i]=time->tm_year%10+'0';
    i++;
    ch[i]=' ';
    i++;
//hour
    if(time->tm_hour<10)
        ch[i]=time->tm_hour%10+'0';
    else
    {
        ch[i]=time->tm_hour/10+'0';
        i++;
        ch[i]=time->tm_hour%10+'0';
    }
    i++;
    ch[i]=':';
    i++;
//minutes
    ch[i]=time->tm_min/10+'0';
    i++;
    ch[i]=time->tm_min%10+'0';
    i++;

    ch[i]=0;
    return ch;
}

void warning(const char *warning_description)
{
    FILE *f=fopen("warning.log", "a");
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo=localtime(&rawtime);

    fprintf(f, get_time(timeinfo));
    fprintf(f, " ");
    fprintf(f, warning_description);
    fprintf(f, "\n");
    fclose(f);
}

void file_not_found(const char *file_name)
{
    FILE *f=fopen("warning.log", "a");
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo=localtime(&rawtime);

    fprintf(f, get_time(timeinfo));
    fprintf(f, " ");
    fprintf(f, "file \"");
    fprintf(f, file_name);
    fprintf(f, "\" not found");
    fprintf(f, "\n");
    fclose(f);
}

void contents_error(const char *file_name)
{
    FILE *f=fopen("warning.log", "a");
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo=localtime(&rawtime);

    fprintf(f, get_time(timeinfo));
    fprintf(f, " ");
    fprintf(f, "contents error for \"");
    fprintf(f, file_name);
    fprintf(f, "\"");
    fprintf(f, "\n");
    fclose(f);
}

void warning_divider()
{
    FILE *f=fopen("warning.log", "a");

    fprintf(f, "\n");

    fclose(f);
}

void error(const char *error_description)
{
    extern char bQuit;
    FILE *f=fopen("warning.log", "a");
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo=localtime(&rawtime);

    bQuit=GL_TRUE;

    fprintf(f, get_time(timeinfo));
    fprintf(f, " ");
    fprintf(f, error_description);
    fprintf(f, "\n");
    fclose(f);

}
