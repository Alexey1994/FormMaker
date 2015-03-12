#ifndef _3DS_H_INCLUDED
#define _3DS_H_INCLUDED

typedef struct
{

}_3DS;

_3DS *load_3DS(const char *name);
void free_3DS(_3DS *_3ds);

#endif // 3DS_H_INCLUDED
