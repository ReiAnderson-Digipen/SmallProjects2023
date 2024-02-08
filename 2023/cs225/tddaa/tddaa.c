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
  void* full = calloc(d1, sizeof(int**) + sizeof(int) * 2);
  int* offsetting = (int*)full;
  int*** ret;
  *offsetting = d1;      /* store d1 in the first 4 bytes of the mem block */
  *(offsetting + 1) = d2;  /* store d2 in the second 4 bytes of the mem block*/
  
  offsetting += 2; 
  ret = (int***)offsetting;
  
  

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
  int* yea = (int*)ppp;
  d2 = *(yea - 1);
  d1 = *(yea - 2);
  for (i = 0; i < d1; i++)
  {
    for (j = 0; j < d2; j++)
    {
      free(ppp[i][j]);
    }
    free(ppp[i]);
  }
  yea -= 2;
  free(yea);
  
}


