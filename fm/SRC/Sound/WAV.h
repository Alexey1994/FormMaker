#ifndef WAV_H_INCLUDED
#define WAV_H_INCLUDED

#include <AL/al.h>
#include <AL/alc.h>

typedef struct
{
    int size_of_sounds;
    ALuint *source;
    ALCdevice *device;
    ALCcontext *context;
}Sound;

Sound* init_OpenAL(int size_of_sounds);
void destroy_OpenAL(Sound *snd);
char* load_WAV(char *name, ALuint *source);

#endif // WAV_H_INCLUDED
