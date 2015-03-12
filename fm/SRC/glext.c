#include "glext.h"

#if defined(_WIN32)

#include <windows.h>
#include <wingdi.h>

#endif

#if defined(__linux)

#include <GL/glx.h>

#endif

#if defined(_WIN32)
#  define glGetProcAddress(name) wglGetProcAddress((LPCSTR)name)
#elif defined(__APPLE__) && !defined(GLEW_APPLE_GLX)
#  define glGetProcAddress(name) NSGLGetProcAddress(name)
#elif defined(__sgi) || defined(__sun)
#  define glGetProcAddress(name) dlGetProcAddress(name)
#elif defined(__ANDROID__)
#  define glGetProcAddress(name) NULL /* TODO */
#elif defined(__native_client__)
#  define glGetProcAddress(name) NULL /* TODO */
#else /* __linux */
#  define glGetProcAddress(name) (*glXGetProcAddressARB)(name)
#endif

void init_gl_extensions()
{
    glUseProgram =          (PFNGLUSEPROGRAMPROC) glGetProcAddress("glUseProgram");
    glCreateShader      = (PFNGLCREATESHADERPROC)    glGetProcAddress("glCreateShader");
    glShaderSource       = (PFNGLSHADERSOURCEPROC)      glGetProcAddress("glShaderSource");
    glCompileShader        = (PFNGLCOMPILESHADERPROC)      glGetProcAddress("glCompileShader");
    glCreateProgram    = (PFNGLCREATEPROGRAMPROC)    glGetProcAddress("glCreateProgram");
    glLinkProgram        = (PFNGLLINKPROGRAMPROC)        glGetProcAddress("glLinkProgram");
    glAttachShader =             (PFNGLATTACHSHADERPROC)      glGetProcAddress("glAttachShader");
    glGenBuffers =               (PFNGLGENBUFFERSPROC) glGetProcAddress("glGenBuffers");
    glBindBuffer =               (PFNGLBINDBUFFERPROC) glGetProcAddress("glBindBuffer");
    glBufferData =               (PFNGLBUFFERDATAPROC) glGetProcAddress("glBufferData");
    glVertexAttribPointer =      (PFNGLVERTEXATTRIBPOINTERPROC) glGetProcAddress("glVertexAttribPointer");
    glEnableVertexAttribArray =  (PFNGLENABLEVERTEXATTRIBARRAYPROC) glGetProcAddress("glEnableVertexAttribArray");
    glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC) glGetProcAddress("glDisableVertexAttribArray");


    glUniform1f          = (PFNGLUNIFORM1FPROC)        glGetProcAddress("glUniform1f");
    glUniform2f          = (PFNGLUNIFORM2FPROC)        glGetProcAddress("glUniform2f");
    glUniform3f          = (PFNGLUNIFORM3FPROC)        glGetProcAddress("glUniform3f");
    glUniform4f          = (PFNGLUNIFORM4FPROC)        glGetProcAddress("glUniform4f");
    glUniform1i          = (PFNGLUNIFORM1IPROC)        glGetProcAddress("glUniform1i");
    glUniform2i          = (PFNGLUNIFORM2IPROC)        glGetProcAddress("glUniform2i");
    glUniform3i          = (PFNGLUNIFORM3IPROC)        glGetProcAddress("glUniform3i");
    glUniform4i          = (PFNGLUNIFORM4IPROC)        glGetProcAddress("glUniform4i");
    glUniform1fv          = (PFNGLUNIFORM1FVPROC)        glGetProcAddress("glUniform1fv");
    glUniform2fv          = (PFNGLUNIFORM2FVPROC)        glGetProcAddress("glUniform2fv");
    glUniform3fv          = (PFNGLUNIFORM3FVPROC)        glGetProcAddress("glUniform3fv");
    glUniform4fv          = (PFNGLUNIFORM4FVPROC)        glGetProcAddress("glUniform4fv");
    glUniform1iv          = (PFNGLUNIFORM1IVPROC)        glGetProcAddress("glUniform1iv");
    glUniform2iv          = (PFNGLUNIFORM2IVPROC)        glGetProcAddress("glUniform2iv");
    glUniform3iv          = (PFNGLUNIFORM3IVPROC)        glGetProcAddress("glUniform3iv");
    glUniform4iv          = (PFNGLUNIFORM4IVPROC)        glGetProcAddress("glUniform4iv");
}

void init_gl_extensions_ARB()
{
    glCreateShaderObjectARB      = (PFNGLCREATESHADEROBJECTARBPROC)    glGetProcAddress("glCreateShaderObjectARB");
    glShaderSourceARB        = (PFNGLSHADERSOURCEARBPROC)      glGetProcAddress("glShaderSourceARB");
    glCompileShaderARB        = (PFNGLCOMPILESHADERARBPROC)      glGetProcAddress("glCompileShaderARB");
    glCreateProgramObjectARB    = (PFNGLCREATEPROGRAMOBJECTARBPROC)    glGetProcAddress("glCreateProgramObjectARB");
    glAttachObjectARB        = (PFNGLATTACHOBJECTARBPROC)      glGetProcAddress("glAttachObjectARB");
    glDetachObjectARB        = (PFNGLDETACHOBJECTARBPROC)      glGetProcAddress("glDetachObjectARB");
    glLinkProgramARB        = (PFNGLLINKPROGRAMARBPROC)        glGetProcAddress("glLinkProgramARB");
    glUseProgramObjectARB      = (PFNGLUSEPROGRAMOBJECTARBPROC)    glGetProcAddress("glUseProgramObjectARB");
    glDeleteObjectARB        = (PFNGLDELETEOBJECTARBPROC)      glGetProcAddress("glDeleteObjectARB");
    glUniform1fARB          = (PFNGLUNIFORM1FARBPROC)        glGetProcAddress("glUniform1fARB");
    glUniform2fARB          = (PFNGLUNIFORM2FARBPROC)        glGetProcAddress("glUniform2fARB");
    glUniform3fARB          = (PFNGLUNIFORM3FARBPROC)        glGetProcAddress("glUniform3fARB");
    glUniform4fARB          = (PFNGLUNIFORM4FARBPROC)        glGetProcAddress("glUniform4fARB");
    glUniform1iARB          = (PFNGLUNIFORM1IARBPROC)        glGetProcAddress("glUniform1iARB");
    glUniform2iARB          = (PFNGLUNIFORM2IARBPROC)        glGetProcAddress("glUniform2iARB");
    glUniform3iARB          = (PFNGLUNIFORM3IARBPROC)        glGetProcAddress("glUniform3iARB");
    glUniform4iARB          = (PFNGLUNIFORM4IARBPROC)        glGetProcAddress("glUniform4iARB");
    glUniform1fvARB          = (PFNGLUNIFORM1FVARBPROC)        glGetProcAddress("glUniform1fvARB");
    glUniform2fvARB          = (PFNGLUNIFORM2FVARBPROC)        glGetProcAddress("glUniform2fvARB");
    glUniform3fvARB          = (PFNGLUNIFORM3FVARBPROC)        glGetProcAddress("glUniform3fvARB");
    glUniform3fvARB          = (PFNGLUNIFORM4FVARBPROC)        glGetProcAddress("glUniform4fvARB");
    glUniform1ivARB          = (PFNGLUNIFORM1IVARBPROC)        glGetProcAddress("glUniform1ivARB");
    glUniform2ivARB          = (PFNGLUNIFORM2IVARBPROC)        glGetProcAddress("glUniform2ivARB");
    glUniform3ivARB          = (PFNGLUNIFORM3IVARBPROC)        glGetProcAddress("glUniform3ivARB");
    glUniform4ivARB          = (PFNGLUNIFORM4IVARBPROC)        glGetProcAddress("glUniform4ivARB");
    glUniformMatrix2fvARB      = (PFNGLUNIFORMMATRIX2FVARBPROC)    glGetProcAddress("glUniformMatrix2fvARB");
    glUniformMatrix3fvARB      = (PFNGLUNIFORMMATRIX3FVARBPROC)    glGetProcAddress("glUniformMatrix3fvARB");
    glUniformMatrix4fvARB      = (PFNGLUNIFORMMATRIX4FVARBPROC)    glGetProcAddress("glUniformMatrix4fvARB");
    glGetObjectParameterfvARB    = (PFNGLGETOBJECTPARAMETERFVARBPROC)  glGetProcAddress("glGetObjectParameterfvARB");
    glGetObjectParameterivARB    = (PFNGLGETOBJECTPARAMETERIVARBPROC)  glGetProcAddress("glGetObjectParameterivARB");
    glGetInfoLogARB          = (PFNGLGETINFOLOGARBPROC)        glGetProcAddress("glGetInfoLogARB");
    glGetAttachedObjectsARB      = (PFNGLGETATTACHEDOBJECTSARBPROC)    glGetProcAddress("glGetAttachedObjectsARB");
    glGetUniformLocationARB      = (PFNGLGETUNIFORMLOCATIONARBPROC)    glGetProcAddress("glGetUniformLocationARB");
    glGetActiveUniformARB      = (PFNGLGETACTIVEUNIFORMARBPROC)    glGetProcAddress("glGetActiveUniformARB");
    glGetUniformfvARB        = (PFNGLGETUNIFORMFVARBPROC)      glGetProcAddress("glGetUniformfvARB");
    glGetUniformivARB        = (PFNGLGETUNIFORMIVARBPROC)      glGetProcAddress("glGetUniformivARB");
    glGetShaderSourceARB      = (PFNGLGETSHADERSOURCEARBPROC)      glGetProcAddress("glGetShaderSourceARB");
}
