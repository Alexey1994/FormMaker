#include "WAV.h"
#include <stdio.h>
#include <stdlib.h>

Sound* init_OpenAL(int size_of_sounds)
{
    Sound *snd=malloc(sizeof(Sound));

    snd->size_of_sounds=size_of_sounds;
    snd->source=malloc(sizeof(ALuint)*size_of_sounds);
    snd->device=alcOpenDevice(NULL);
    snd->context=alcCreateContext(snd->device, NULL);
    alcMakeContextCurrent(snd->context);

    return snd;
}

void destroy_OpenAL(Sound *snd)
{
    int i;
    for(i=0; i<snd->size_of_sounds; i++)
        alDeleteSources(snd->size_of_sounds, snd->source);

    alcDestroyContext(snd->context);
    alcCloseDevice(snd->device);
}

char* load_WAV(char *name, ALuint *source)
{
    FILE *fp = NULL;

    char type[4];
    ALuint size,chunkSize;
    short formatType,channels;
    ALuint sampleRate,avgBytesPerSec;
    short bytesPerSample,bitsPerSample;
    ALuint dataSize;
    unsigned char* buf;

    ALuint buffer;
    ALuint frequency;
    ALenum format=0;

    ALfloat SourcePos[] = { 0.0, 0.0, 0.0 };
    ALfloat SourceVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerPos[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
    ALfloat ListenerOri[] = { 0.0, 0.0, -1.0,  0.0, 1.0, 0.0 };

    fp=fopen(name, "rb");
    if (!fp)
        return "error opening";

    fread(type,sizeof(char),4,fp);
    if(type[0]!='R' || type[1]!='I' || type[2]!='F' || type[3]!='F')
        return "No RIFF";

    fread(&size, sizeof(ALuint),1,fp);
    fread(type, sizeof(char),4,fp);
    if (type[0]!='W' || type[1]!='A' || type[2]!='V' || type[3]!='E')
        return "not WAVE";

    fread(type,sizeof(char),4,fp);
    if (type[0]!='f' || type[1]!='m' || type[2]!='t' || type[3]!=' ')
        return "not fmt ";

    fread(&chunkSize,sizeof(ALuint),1,fp);
    fread(&formatType,sizeof(short),1,fp);
    fread(&channels,sizeof(short),1,fp);
    fread(&sampleRate,sizeof(ALuint),1,fp);
    fread(&avgBytesPerSec,sizeof(ALuint),1,fp);
    fread(&bytesPerSample,sizeof(short),1,fp);
    fread(&bitsPerSample,sizeof(short),1,fp);

    fread(type,sizeof(char),4,fp);
    if (type[0]!='d' || type[1]!='a' || type[2]!='t' || type[3]!='a')
        return "Missing DATA";

    fread(&dataSize, sizeof(ALuint), 1, fp);

    buf=malloc(sizeof(unsigned char)*dataSize);
    fread(buf, sizeof(ALubyte), dataSize, fp);

    frequency=sampleRate;

    alGenBuffers(1, &buffer);
    alGenSources(1, source);
    if(alGetError() != AL_NO_ERROR) return "Error GenSource";

    if(bitsPerSample == 8)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO8;
        else if(channels == 2)
            format = AL_FORMAT_STEREO8;
    }
    else if(bitsPerSample == 16)
    {
        if(channels == 1)
            format = AL_FORMAT_MONO16;
        else if(channels == 2)
            format = AL_FORMAT_STEREO16;
    }
    if(!format) return "Wrong BitPerSample";

    alBufferData(buffer, format, buf, dataSize, frequency);
    if(alGetError() != AL_NO_ERROR)
        return "Error loading ALBuffer";

    alListenerfv(AL_POSITION,    ListenerPos);
    alListenerfv(AL_VELOCITY,    ListenerVel);
    alListenerfv(AL_ORIENTATION, ListenerOri);

    alSourcei (*source, AL_BUFFER,   buffer);
    alSourcef (*source, AL_PITCH,    1.0f     );
    alSourcef (*source, AL_GAIN,     1.0f     );
    alSourcefv(*source, AL_POSITION, SourcePos);
    alSourcefv(*source, AL_VELOCITY, SourceVel);
    alSourcei (*source, AL_LOOPING,  AL_FALSE );

    fclose(fp);
    free(buf);
    return 0;
}
