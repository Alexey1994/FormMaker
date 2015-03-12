#include "AVI.h"
#include <vfw.h>
#include <gl\gl.h>
#include <gl\glext.h>
#include "../Render/Render.h"
#include "../Error/Error.h"

GLfloat video_aspect;

int frame=0;

AVISTREAMINFO psi;
PAVISTREAM pavi;
PGETFRAME pgf; // Указатель на объект

BITMAPINFOHEADER bmih;
long lastframe;
int width;
int height;
char *pdata;
int mpf;

HDRAWDIB hdd;
HBITMAP hBitmap;
unsigned char* data = 0;

void OpenAVI(LPCSTR szFile)
{
    AVIFileInit();

    if (AVIStreamOpenFromFile(&pavi, szFile, streamtypeVIDEO, 0, OF_READ, NULL) !=0)
        MessageBox (HWND_DESKTOP, "Failed To Open The AVI Stream", "Error", MB_OK | MB_ICONEXCLAMATION);

    width=psi.rcFrame.right-psi.rcFrame.left;
    height=psi.rcFrame.bottom-psi.rcFrame.top;
    lastframe=AVIStreamLength(pavi);
    mpf=AVIStreamSampleToTime(pavi,lastframe)/lastframe;

    bmih.biSize = sizeof (BITMAPINFOHEADER);
    bmih.biPlanes = 1;
    bmih.biBitCount = 24;
    bmih.biWidth = 640;
    bmih.biHeight = 480;
    bmih.biCompression = BI_RGB;

    video_aspect=(GLfloat)bmih.biWidth/(GLfloat)bmih.biHeight;

    pgf=AVIStreamGetFrameOpen(pavi, NULL);

    if (pgf==NULL)
        warning("Failed To Open The AVI Frame");
}

void GrabAVIFrame(int frame)
{
    LPBITMAPINFOHEADER lpbi;
    lpbi = (LPBITMAPINFOHEADER)AVIStreamGetFrame(pgf, frame);

    pdata=(char *)lpbi+lpbi->biSize+lpbi->biClrUsed *sizeof(RGBQUAD);
    glTexSubImage2D (GL_TEXTURE_2D, 0, 0, 0, 640, 480, GL_BGR, GL_UNSIGNED_BYTE, pdata);
}

void open_AVI(const char *name)
{
    hdd = DrawDibOpen();

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    OpenAVI(name);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 640, 480, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
}

void close_AVI()
{
    DeleteObject(hBitmap);
    DrawDibClose(hdd);
    AVIStreamGetFrameClose(pgf);
    AVIStreamRelease(pavi);
    AVIFileExit();
}

void play_AVI()
{
    GrabAVIFrame(frame);
    draw_box(-video_aspect, -1, video_aspect, 1);

    frame++;

    if(frame>=lastframe)
        frame=0;
}
