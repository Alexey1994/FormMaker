#include <stdlib.h>
#include <stdio.h>
#include "../Image.h"
#include "ImageType.h"
#include "../../Math.h"
#include "../../Error/Error.h"
#include "../../compression.h"
#include <GL/glext.h>

Image *load_image(const char *name)
{
    Image *image;
    FILE *f=fopen(name, "rb");

    image=image_loader(f);

    return image;
}

Image *image_loader(FILE *f)
{
    Image *image=0;

    if(f==0)
        return 0;

    image=TGA_loader(f);

    if(image==0)
    {
        rewind(f);
        image=BMP_loader(f);
    }
    if(image==0) return 0;

    return image;
}

void free_image(Image *img)
{
    free(img->data);
    free(img);
}

Image *TGA_loader(FILE *f)
{
    TGA tga;
    Image *image=0;
    short count;
    GLubyte color_buffer[4], chunk_header;
    GLuint pixel_count, current_pixel=0, current_byte=0;

    GLubyte compressed[]= {0,0,10,0,0,0,0,0,0,0,0,0},
            uncompressed[]= {0,0,2,0,0,0,0,0,0,0,0,0},
            head[12];


    if(fread(head, 12, 1, f)==0) return 0;
    if(fread(tga.header, 6, 1, f)==0) return 0;

    tga.width=tga.header[1]*256+tga.header[0];
    tga.height=tga.header[3]*256+tga.header[2];
    tga.bpp=tga.header[4];

    if(tga.width<1 || tga.height<1 || (tga.bpp!=32 && tga.bpp!=24)) return 0;

    image=malloc(sizeof(Image));

    image->height=tga.height;
    image->width=tga.width;
    if(tga.bpp==32) image->bpp=GL_RGBA;
    if(tga.bpp==24) image->bpp=GL_RGB;

    tga.bpp/=8;
    tga.image_size=tga.height*tga.width*tga.bpp;

    if(memory_comparision(head, compressed, 12))
    {
        image->data=(GLubyte*)malloc(tga.image_size);
        if(image->data==0)
        {
            free(image);
            return 0;
        }

        pixel_count=image->width * image->height;

        do
        {
            chunk_header=0;

            if(fread(&chunk_header, 1, 1, f)!=1)
            {
                free(image);
                return 0;
            }

            if(chunk_header<128)   //RAW
            {
                chunk_header++;

                for(count=0; count<chunk_header; count++)
                {
                    if(fread(color_buffer, 1, tga.bpp, f)!=tga.bpp)
                    {
                        free(image->data);
                        free(image);
                        return 0;
                    }

                    image->data[current_byte]=color_buffer[0];
                    image->data[current_byte+1]=color_buffer[1];
                    image->data[current_byte+2]=color_buffer[2];
                    if(tga.bpp==4) image->data[current_byte+3]=color_buffer[3];

                    current_byte+=tga.bpp;
                    current_pixel++;
                }
            }
            else    //RLE
            {
                chunk_header -= 127;

                if(fread(color_buffer, 1, tga.bpp, f)!=tga.bpp)
                {
                    free(image->data);
                    free(image);
                    return 0;
                }

                for(count=0; count<chunk_header; count++)
                {
                    image->data[current_byte]=color_buffer[0];
                    image->data[current_byte+1]=color_buffer[1];
                    image->data[current_byte+2]=color_buffer[2];
                    if(tga.bpp==4) image->data[current_byte+3]=color_buffer[3];

                    current_byte+=tga.bpp;
                }

                current_pixel+=chunk_header;
            }
        }
        while(current_pixel < pixel_count);
    }
    else if(memory_comparision(head, uncompressed, 12))
    {
        (image->data)=(GLubyte*)malloc(tga.image_size);
        if(image->data==0)
        {
            free(image);
            return 0;
        }

        if(fread(image->data, 1, tga.image_size, f)!=tga.image_size)
        {
            free(image->data);
            free(image);
            return 0;
        }
    }
    else return 0;

    return image;
}

void save_TGA(FILE *f, const char *is_compressed, Image *image)
{
    unsigned int image_size;
    GLubyte compressed[]= {0,0,10,0,0,0,0,0,0,0,0,0},
            uncompressed[]= {0,0,2,0,0,0,0,0,0,0,0,0},
            bpp, tmp=0;
    GLshort width=image->width, height=image->height;

    if(image->bpp==GL_RGB)
        bpp=24;
    else if(image->bpp==GL_RGBA)
        bpp=32;
    else warning("i can't save: unsupported image data(expect GL_RGB or GL_RGBA)");

    if(string_comparision("uncompressed", is_compressed))
    {
        fwrite(uncompressed, 1, 12, f);
        fwrite(&width, 2, 1, f);
        fwrite(&height, 2, 1, f);
        fwrite(&bpp, 1, 1, f);
        fwrite(&tmp, 1, 1, f);
        fwrite(image->data, 1, bpp/8*image->width*image->height, f);
    }
    else
    {
        fwrite(compressed, 1, 12, f);
        fwrite(&width, 2, 1, f);
        fwrite(&height, 2, 1, f);
        fwrite(&bpp, 1, 1, f);
        fwrite(&tmp, 1, 1, f);
        bpp/=8;
        image_size=width*height;

        RLE(image->data, bpp, image_size, f);
    }
}

Image *load_BMP(const char *name)
{
    Image *image;
    FILE *f=fopen(name, "rb");

    image=BMP_loader(f);

    fclose(f);
    return image;
}

Image *BMP_loader(FILE *f)
{
    Image *image;
    char verify[]="BM";
    BMP bmp;
    int i, size;

    //BITMAPFILEHEADER
    if(fread(bmp.type, 2, 1, f)==0) return 0;
    if(memory_comparision((GLubyte*)verify, bmp.type, 2)!=1) return 0;

    if(fread(&bmp.size, 1, sizeof(GLuint), f)==0) return 0;
    if(fread(&bmp.reserved1, 1, sizeof(GLushort), f)==0) return 0;
    if(bmp.reserved1!=0) return 0;
    if(fread(&bmp.reserved2, 1, sizeof(GLushort), f)==0) return 0;
    if(bmp.reserved2!=0) return 0;
    if(fread(&bmp.off_bits, 1, sizeof(GLuint), f)==0) return 0;

    //BITMAPINFO
    if(fread(&bmp.version, 1, sizeof(GLuint), f)==0) return 0;
    if(fread(&bmp.width, 1, sizeof(GLint), f)==0) return 0;
    if(fread(&bmp.height, 1, sizeof(GLint), f)==0) return 0;
    if(fread(&bmp.planes, 1, sizeof(GLushort), f)==0) return 0;
    if(fread(&bmp.bpp, 1, sizeof(GLushort), f)==0) return 0;
    switch(bmp.version)
    {
        case 12:
                break;

        case 40:
                if(fread(&bmp.compression, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.size_image, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.x_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.y_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_used, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_important, 1, sizeof(GLuint), f)==0) return 0;
                break;
        case 108:
                if(fread(&bmp.compression, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.size_image, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.x_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.y_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_used, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_important, 1, sizeof(GLuint), f)==0) return 0;
                for(i=0; i<68; i++) fgetc(f);
                break;
        case 124:
                if(fread(&bmp.compression, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.size_image, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.x_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.y_pels_per_meter, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_used, 1, sizeof(GLuint), f)==0) return 0;
                if(fread(&bmp.clr_important, 1, sizeof(GLuint), f)==0) return 0;
                for(i=0; i<84; i++) fgetc(f);
    }

    size=3*bmp.width*bmp.height;
    image=(Image*)malloc(sizeof(Image));
    image->width=bmp.width;
    image->height=bmp.height;
    image->bpp=GL_RGB;
    image->data=(GLubyte*)malloc(size);

    for(i=0; i<bmp.off_bits-bmp.version-14; i++) fgetc(f);

    if(fread(image->data, size, 1, f)==0)
    {
        free(image->data);
        free(image);
        return 0;
    }

    return image;
}

void save_BMP(FILE *f)
{

}

Image *load_JPEG(const char *name)
{
    Image *image;
    FILE *f=fopen(name, "rb");

    image=JPEG_loader(f);

    fclose(f);
    return image;
}

Image *JPEG_loader(FILE *f)
{
    Image *image;
    JPEG jpg;
    int i, size;



    return image;
}

void save_JPEG(FILE *f)
{

}

Image *load_PNG(const char *name)
{
    Image *image;
    FILE *f=fopen(name, "rb");

    image=PNG_loader(f);

    fclose(f);
    return image;
}

Image *PNG_loader(FILE *f)
{
    Image *image;
    PNG png;
    int i, size;



    return image;
}

void save_PNG(FILE *f)
{

}
