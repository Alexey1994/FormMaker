#ifndef SHADER_H_INCLUDED
#define SHADER_H_INCLUDED

#include <GL/gl.h>
#include <GL/glext.h>
#include "glext.h"

GLuint load_shader(const char *vs_name, const char *fs_name);

#endif // SHADER_H_INCLUDED
