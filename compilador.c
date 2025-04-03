/*
Integrantes: Raphael Iniesta Reis
RA: 10396285
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "LinkedList.h"
#include "Tokens.h"
#include "Lexico.h"

int main(int argc, char *argv[]) {
   LinkedList *list = new_list();
   FILE *file = fopen(argv[1], "r");
   int line = 1;

   if(file == NULL) {
      fclose(file);
      free(list);
      printf("Failed to open file\n");
      exit(EXIT_FAILURE);
   }

   char *buffer = read_file(file);

   Analisador_Lexico(list, &buffer, &line);

   free_list(list); // Limpa e libera todos os nós da lista, incluindo a lista
   return 0;
}