#include "compression.h"
#include "Math.h"
#include <stdlib.h>

void RLE(const GLubyte *data, int size_of, int size_data, FILE *f)
{
    int i, j;
    GLubyte chunk=0, unchunk=0;

    for(i=0; i<size_data-1; i++)
    {
        if(memory_comparision(data+i*size_of, data+((i+1)*size_of), size_of))
        {
            chunk++;
            if(chunk==128)
            {
                chunk+=127;
                fwrite(&chunk, 1, 1, f);
                fwrite(data+i*size_of, size_of, 1, f);
                chunk=0;
            }
        }
        else
        {
            if(chunk!=0)
            {
                chunk+=127;
                fwrite(&chunk, 1, 1, f);
                fwrite(data+(i-1)*size_of, size_of, 1, f);
                chunk=0;
            }

            for(j=0; j<128; j++)
            {
                if(memory_comparision(data+(i+j)*size_of, data+((i+j+1)*size_of), size_of))
                    break;

                unchunk++;
            }

            if(unchunk>0)
            {
                unchunk--;
                fwrite(&unchunk, 1, 1, f);
                unchunk++;
                for(j=0; j<unchunk; j++)
                {
                    fwrite(data+i*size_of, size_of, 1, f);
                    i++;
                    if(i>=size_data-1) break;
                }
                i--;
                unchunk=0;
            }
        }
    }

    if(chunk>0)
    {
        chunk+=127;
        fwrite(&chunk, size_of, 1, f);
        fwrite(data+size_data-1, size_of, 1, f);
    }

    fputc(0, f);
    fwrite(data+size_data, size_of, 1, f);
}
