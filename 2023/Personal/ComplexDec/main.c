#include <stdlib.h>

int main(void){
  int (*var)[32];
  var = malloc(sizeof(int) * 64);
  (*var)[32] = 0;
  free(var);
  return 0;
}