/*
2x3x4
+----------------------------------------+
|+------------------++------------------+|
||+----++----++----+||+----++----++----+||
|||1234||1234||1234||||1234||1234||1234|||
||+----++----++----+||+----++----++----+||
|+------------------++------------------+|
+----------------------------------------+

+------+
|abcdef| 6 pointers to 1's ( array of row pointers )
+------+

+--+
|mn| 2 pointers to a and d above ( array of pointers to slices )
+--+

a[i][j][k]
i chooses which slice
j chooses which row within the slice
k chooses within element with the row
*/

#include "tddaa.h"
#include <stdlib.h>

int *** allocate( int d1, int d2, int d3 ) 
{
  int i, j;
  int*** ret = (int***)calloc(d1, sizeof(int**) + sizeof(int) * 2);
  *((int*)ret) = d1;      /* store d1 in the first 4 bytes of the mem block */
  *((int*)ret + 1) = d2;  /* store d2 in the second 4 bytes of the mem block*/
  ret += 1; /*a pointer is the size of two ints lol*/
  
  for (i = 0; i < d1; i++)
  {
    ret[i] = (int**)calloc(d2, sizeof(int*));
    for (j = 0; j < d2; j++)
    {
      ret[i][j] = (int*)calloc(d3, sizeof(int));
    }
  }
  return ret;
}

void deallocate( int *** ppp ) 
{
  int d1, d2;
  int i, j;
  d2 = *((int*)ppp - 1);
  d1 = *((int*)ppp - 2);
  for (i = 0; i < d1; i++)
  {
    for (j = 0; j < d2; j++)
    {
      free(ppp[i][j]);
    }
    free(ppp[i]);
  }
  ppp--;
  free(ppp);
  (void)ppp;
}


