#include "fuse.h"
#include <stdio.h>    /* fopen/fclose/fread/fwrite/fseek/ftell */
#include <stdlib.h>   /* malloc/free/exit */
#include <string.h>   /* strlen */

#define xDEBUG

static void find_file_data( FILE * opened_file, long unsigned int *file_size, char *data_buffer );

int fuse( char const ** filenames, int num_files, char const * output)
{  
  int BUFFER_SIZE = 1<<16; /* 65 Kb */
  
  /* char * file_name_buffer[256]; //might need */
  FILE *opened_file;
  long unsigned int file_size = 0;
  char *data_buffer;
  FILE *result;
  int i;

  data_buffer = (char *)malloc( BUFFER_SIZE * sizeof(char) );


  result = fopen( output, "wb" );
  if(result == NULL)
  {
    return E_BAD_DESTINATION;
  }


  /* loop thro each file and open it and grab its name, size and data while writing it
  to a new file */
  for ( i = 0; i < num_files; ++i )
  {

    /* read file name into file name buffer thro fget or fread */
    opened_file = fopen( filenames[i], "rb" ); 
    if(opened_file == NULL)
    {
      fclose(result);
      return E_BAD_SOURCE;
    }
    fwrite( filenames[i], sizeof(char), strlen(filenames[i]) + 1, result );
    find_file_data(opened_file, &file_size, data_buffer);

    /*while ( !feof(opened_file) )
    {
      file_size = fread(data_buffer, 1, file_size, opened_file);
      fwrite( data_buffer, 1, (size_t)file_size, result );
    }*/
    

    fclose(opened_file);

 
    fwrite( &file_size, sizeof(int), 1, result );

    fwrite( data_buffer, 1, (size_t)file_size, result );
    
    

  }
    fclose(result);
    free(data_buffer);
    return 1;
}

int unfuse( char const * filename )
{
  int NAME_SIZE = 256;
  int BUFFER_SIZE = 1<<16; /* 65 Kb */

  char *name_buffer;
  char *data_buffer;

  FILE *opened_file;
  FILE *new_out_file;

  int file_data_size = 0;

  int i;
  
  /*malloc buffer*/
  name_buffer = (char *)malloc(NAME_SIZE * sizeof(char));
  data_buffer = (char *)malloc( BUFFER_SIZE * 15);

  /*open source file*/
  opened_file = fopen( filename, "rb" );

  if(opened_file == NULL)
  {
    return E_BAD_SOURCE;
  }

  
  while ( !feof(opened_file) )
  {
    for(i = 0; i < NAME_SIZE; ++i)
    {
      fread(name_buffer+i, sizeof(char), 1, opened_file);
      if(name_buffer[i] == '\0')
      {
        break;
      }
    }

    /*fopen new file with name*/
    new_out_file = fopen( name_buffer, "wb" );

    if(new_out_file == NULL)
    {
      return E_BAD_DESTINATION;
    }

    printf("%d  1  \n", file_data_size);
    fflush(stdout);

    /* grab file size */
    fread( &file_data_size, sizeof(int), 1, opened_file );

    fwrite(data_buffer, sizeof(char), file_data_size, opened_file);

    printf("%d  2  \n", file_data_size);
    fflush(stdout);

  }

      return 1;
}

static void find_file_data( FILE * opened_file, long unsigned int *file_size, char *data_buffer )
{
  int FSEEK_OFFSET = 0; /* no offset */
  int BUFFER_SIZE = 1<<16; /* 65 Kb */

    /* gives me file size */
    fseek(opened_file, FSEEK_OFFSET, SEEK_END);

    *file_size = (int)ftell(opened_file);


    /* rewind the string back to the start */
    rewind( opened_file );

    fread( data_buffer, 1, BUFFER_SIZE, opened_file );

}
