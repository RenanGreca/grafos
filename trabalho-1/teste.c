#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------
int main(void) {
/*
  FILE *file_out = fopen("c.out","w+");

  FILE *file_in = fopen("cidades.dot","r");
  if (!file_in)
      return -1;
*/
//  int r = destroi_grafo(escreve_grafo(file_out, le_grafo(file_in)));
/*
  fclose(file_out);
  fclose(file_in);
*/
 // return r;
  return ! destroi_grafo(escreve_grafo(stdout, le_grafo(stdin)));
}
