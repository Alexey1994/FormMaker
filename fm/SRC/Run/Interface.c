#include "Interface.h"

#if defined(WIN32)

#include <windows.h>

char key(char key)
{
    if(GetAsyncKeyState(key)) return 1;
    return 0;
}

#endif //WIN32

#if defined(__linux__)

char key(char key)
{
    return 0;
}

#endif //LINUX

char key_up(unsigned char k)
{
    static char buf[256]={0};
    int i;

    for(i=0; i<256; i++)
    {
        if(buf[i]==1 && k==i && !key(k))
        {
            buf[i]=0;
            return 1;
        }
    }
    if(key(k)) buf[(int)k]=1;

    return 0;
}

char key_code()
{
    unsigned char i=0;

    for(i=0; i<255; i++)
        if(key_up(i)) return i;

    return 0;
}
