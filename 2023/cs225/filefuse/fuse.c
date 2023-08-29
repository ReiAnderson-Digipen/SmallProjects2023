#include "fuse.h"
#include <stdio.h>    /* fopen/fclose/fread/fwrite */
#include <stdlib.h>   /* malloc/free */
#include <string.h>   /* strlen */

#define xDEBUG
#define chunk_size 256 /* for max filename size and chunk grab size on file */
int fuse( char const ** filenames, int num_files, char const * output)
{
 
  int i = 0;                        // filenname index
  const char nulchar = 0;           // placeholder to write null character
  FILE* out = fopen(output, "wb");  // Output file 
  if (!out) // Test if output file opened successfully
  {
    return E_BAD_DESTINATION;
  }

  // run through all the files
  for (i = 0; i < num_files; i++)
  {
    FILE* file = fopen(filenames[i], "rb"); // current file at index i
    size_t nameLen = strlen(filenames[i]);  // length of the filename
    size_t fileSize;                        // sizeof the file
    size_t read_left;                       // how many bytes left to read

    // Check if the file opened successfully
    if (!file)
    {
      // Make sure to close the output file if input doesnt open correctly
      fclose(out);
      return E_BAD_SOURCE;
      
    }

    // seek to the end to check filesize
    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    // reset back to the beginning 
    fseek(file, 0, SEEK_SET);

    // write the filename
    fwrite(filenames[i], sizeof(char), nameLen, out);
    // write the null terminator 
    fwrite(&nulchar, sizeof(char), 1, out);
    // write file size
    fwrite(&fileSize, sizeof(int), 1, out);

    // set how many bytes left in file
    read_left = fileSize;

    // while there are still bytes to read, keep reading
    while (read_left > 0)
    {
      
      char buf[chunk_size];        // buffer to read data to
      int grab = chunk_size;       // current size to grab
      if (read_left < chunk_size)
      {
        grab = read_left; // if the amount of bytes is less than chunk_size, you should only read how many left
      }
      // read 'grab' amount of bytes from file
      fread(buf, sizeof(char), grab, file);
      // and write it to the output file
      fwrite(buf, sizeof(char), grab, out);
      // subtract 'grab' from the amount of bytes left to read
      read_left -= grab;

    }
    fclose(file);
    
  }
  fclose(out);
  return 0;
}

int unfuse( char const * filename )
{
  
  char new_filename[chunk_size];        // filenames were told to not be bigger than 256 characters
  FILE* infile = fopen(filename, "rb"); // input fused file
  int hitEnd = 0;                       // flag to break out of loop

  // Check if the file opened right
  if (!infile)
  {
    return E_BAD_SOURCE;
  }
  
  //while the file hasnt hit the end
  while (!hitEnd)
  {
    int i = 0;               // index in new_filename
     
    int grab = chunk_size;   // current chunk size to grab from the file
    int read_left = 0;       // how many bytes in subfile left
    char buffer[chunk_size]; // buffer to read into

    FILE* current_file;      // file pointer to the created file

    char c = 1;              // character to use to search for null terminator

    //while c isnt null terminator and we havent hit the end of the file
    while (c != '\0' && !hitEnd)
    {
      c = fgetc(infile);
      new_filename[i] = c;
      i++;
      hitEnd = feof(infile);
    }

    // open/create the subfile
    current_file = fopen(new_filename, "wb");
    // read the subfile-size from the fused file
    fread(&read_left, sizeof(int), 1, infile);

    // while we have bytes left to read
    while (read_left > 0)
    {
      // if the count of bytes is less than chunk size
      if (read_left < chunk_size)
      {
        grab = read_left; //simply read how many bytes left
      }
      else
      {
        grab = chunk_size; //otherwise, read chunk size
      }

      // read the data
      fread(buffer, sizeof(char), grab, infile);
      // write that data to the unfuzed file
      fwrite(buffer, sizeof(char), grab, current_file);
      // reduce bytes left to read
      read_left -= grab;
    }
    fclose(current_file);
    // check for end of file
    hitEnd = feof(infile);
  }
  return 0;
}
