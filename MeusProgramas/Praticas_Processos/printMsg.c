#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/** Imprime o primeiro argumento passado para o programa **/

int main(int argc, char* argv[]) {

   if(argc > 1) {
      int i = 1;
      for(i; i < argc; i++) {
         printf("\n\tArgumento %d: %s\n", i, argv[i]);
      }
   } else {
      perror("O programa precisa de ao menos um parâmetro.");
      exit(-1);
   }

   exit(0);
}
