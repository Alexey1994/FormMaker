#include "3DS.h"
#include <stdlib.h>
#include <stdio.h>

_3DS *load_3DS(const char *name)
{
    _3DS *_3ds;
    FILE *f=fopen(name, "rb");

    if(f==0)
        return 0;

    _3ds=malloc(sizeof(_3DS));

    while(1)
    {
        if(feof(f)) break;
    }

    fclose(f);
    return _3ds;
}

void free_3DS(_3DS *_3ds)
{

}
