#include "shader.h"
#include <stdio.h>
#include <stdlib.h>

char *read_shader_file(const char *name)
{
    FILE *f=fopen(name, "rb");
    int end_file=0;
    char *buf;

    if(f==0) return 0;

    while(!feof(f))
    {
        fgetc(f);
        end_file++;
    }

    fclose(f);

    f=fopen(name, "rb");

    buf=malloc(end_file);
    fread(buf, 1, end_file, f);
    buf[end_file]='\0';

    fclose(f);
    return buf;
}

void print_error(GLhandleARB program)
{
    char s[2560];
    glGetInfoLogARB(program, 2560, 0,s);
    printf(s);
}

GLuint load_shader(const char *vs_name, const char *fs_name)
{
    GLuint program;
    GLhandleARB vs, fs;
    char *vs_string[1], *fs_string[1];

    vs_string[0]=read_shader_file(vs_name);
    fs_string[0]=read_shader_file(fs_name);

    vs=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, (const char**)vs_string, 0);
    glCompileShader(vs);
    print_error(vs);

    fs=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, (const char**)fs_string, NULL);
    glCompileShader(fs);
    print_error(fs);

    program=glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    print_error(program);

    free(vs_string[0]);
    free(fs_string[0]);
    return program;
}
