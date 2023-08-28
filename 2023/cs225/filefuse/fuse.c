#include "fuse.h"
#include <stdio.h>    /* fopen/fclose/fread/fwrite */
#include <stdlib.h>   /* malloc/free */
#include <string.h>   /* strlen */

#define xDEBUG
#define chunk_size 256
int fuse( char const ** filenames, int num_files, char const * output)
{
 
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
  return 0;
}

int unfuse( char const * filename )
{
  char new_filename[chunk_size];
  FILE* infile = fopen(filename, "rb");
  int hitEnd = 0;
  if (!infile)
  {
    return E_BAD_SOURCE;
  }
  while (!hitEnd)
  {
    int i = 0;
    
    int grab = chunk_size;
    int read_left = 0;
    char buffer[chunk_size];

    FILE* current_file;

    char c = 1;
    while (c != '\0' && !hitEnd)
    {
      c = fgetc(infile);
      new_filename[i] = c;
      i++;
      hitEnd = feof(infile);
    }
    current_file = fopen(new_filename, "wb");
    fread(&read_left, sizeof(int), 1, infile);
    while (read_left > 0)
    {
      if (read_left < chunk_size)
      {
        grab = read_left;
      }
      else
      {
        grab = chunk_size;
      }
      fread(buffer, sizeof(char), grab, infile);
      fwrite(buffer, sizeof(char), grab, current_file);
      read_left -= grab;
    }
    fclose(current_file);
    hitEnd = feof(infile);
  }
  return 0;
}
