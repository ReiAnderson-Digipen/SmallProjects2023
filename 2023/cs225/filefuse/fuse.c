#include "fuse.h"
#include <stdio.h>    /* fopen/fclose/fread/fwrite */
#include <stdlib.h>   /* malloc/free */
#include <string.h>   /* strlen */

#define xDEBUG

int fuse( char const ** filenames, int num_files, char const * output)
{
  int BUFFER_SIZE = 1 << 16; /* 65 Kb */
  int i = 0;
  const char nulchar = 0;
  FILE* out = fopen(output, "wb");
  if (!out)
  {
    return E_BAD_DESTINATION;
  }
  for (i = 0; i < num_files; i++)
  {
    FILE* file = fopen(filenames[i], "rb");
    size_t nameLen = strlen(filenames[i]);
    size_t fileSize;
    size_t read_left;
    if (!file)
    {
      fclose(out);
      return E_BAD_SOURCE;
      
    }
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    
    fseek(file, 0, SEEK_SET);
    
    fwrite(filenames[i], sizeof(char), nameLen, out);
    fwrite(&nulchar, sizeof(char), 1, out);
    fwrite(&fileSize, sizeof(int), 1, out);

    read_left = fileSize;
    while (read_left > 0)
    {
      const int chunk_size = 256;
      char buf[chunk_size];
      int grab = chunk_size;
      if (read_left < chunk_size)
      {
        grab = read_left;
      }
      fread(buf, sizeof(char), grab, file);
      fwrite(buf, sizeof(char), grab, out);
      read_left -= grab;

    }
    fclose(file);
    
  }
  fclose(out);
}

int unfuse( char const * filename )
{
  while (1)
  {
    
  }
}
