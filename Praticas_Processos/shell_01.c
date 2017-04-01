#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/** Um pequeno shell **/

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 16
#define BT_ENTER 10
#define PROMPT "\n-DigiteUmComando: "

void typePrompt();
void clearBuffer(char* bf, int length);
void readCommand(char* bf, int lenght);

char rdCmdBuffer[BUFFER_SIZE];

int main(int argc, char* argv[])
{

   pid_t cId;
   int cStatus;

   while(strcmp(rdCmdBuffer, "exit")) {
      clearBuffer(rdCmdBuffer, BUFFER_SIZE);
      typePrompt();
      readCommand(rdCmdBuffer, BUFFER_SIZE);

      cId = fork();
      if(cId != 0) {
         wait(&cStatus);
      } else {
         execv(rdCmdBuffer, argv);
      }
   }

   exit(0);
}

void typePrompt()
{
   char sPrompt[21];

   strcpy(sPrompt, PROMPT);

   fputs(sPrompt, stdout);

   return;
}

void clearBuffer(char* bf, int lenght)
{
   int i = 0;
   for(i; i < lenght; i++) {
      bf[i] = '\0';
   }
   return;
}

/** Lê os comandos digitados pelo usuário até o limite definido em lenght **/
void readCommand(char* bf, int lenght)
{
   int i = 0;
   char c = 0;
   while(TRUE) {
      if(i < lenght) {
         c = getchar();
         if(c != BT_ENTER) {
            bf[i] = c;
         } else {
            break;
         }
         i++;
      } else {
         i = 0; //Corrigir isso para exibir uma mensagem de erro!
      }
   }
   return;
}
