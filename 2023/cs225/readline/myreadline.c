#include <stdlib.h> /* malloc/calloc, free */
#include <string.h> /* strlen */
#include "readline.h"


struct buffer {
  unsigned size, capacity;
  char* data;
}; 

/*
resize_buf, takes a buffer, allocates a new buffer double the size of
its capacity, copies data from buffer
to the new buffer, frees the old buffer, and sets the buffer data pointer to the new buffer data
*/
static void resize_buf(struct buffer* buffer)
{
  
  char* currentDat = buffer->data;
  if (currentDat != NULL)
  {
    buffer->data = (char*)calloc(buffer->capacity * 2, sizeof(char));
    for (unsigned i = 0; i < buffer->capacity; i++)
    {
      buffer->data[i] = currentDat[i];
    }
    buffer->capacity = buffer->capacity * 2;
    free(currentDat);
  }
  
}


static struct buffer buffer_create()
{
  struct buffer b = { 0, 2, (char*)calloc(2, sizeof(char)) };
  return b;
}

char* readline(FILE* f)
{
  struct buffer linebuf = buffer_create();
  while (1)
  {
    char* unused = fgets(linebuf.data + linebuf.size, linebuf.capacity - linebuf.size, f);
    (void)unused;
    resize_buf(&linebuf);
    linebuf.size = strlen(linebuf.data);
    if (*(linebuf.data + linebuf.size - 1) == '\n')
    {
      *(linebuf.data + linebuf.size - 1) = 0;
      break;
    }
        
    if (feof(f))
    {
      break;
    }

  }
  return linebuf.data;
}



