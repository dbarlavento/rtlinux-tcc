#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
 * Este programa lança varias instâncias de um processo com diferentes prioridades
 * e periodos de ativação.
 */

#define TRUE 1
#define FALSE 0
#define BUFFER_SIZE 16 //Tamanho do buffer de leitura do que é digitado pelo usuário
#define BT_ENTER 10 //ASCII em inteiro da tecla "enter"
#define PROMPT "\n-DigiteUmComando: " //String do prompt

void typePrompt();
void clearBuffer(char* bf, int length);
void readCommand(char* bf, int lenght);

char rdCmdBuffer[BUFFER_SIZE]; //Buffer de leitura do que é digitado pelo usuário

//Inicio
int main(int argc, char* argv[])
{

    pid_t cId;
    int cStatus;

    while(strcmp(rdCmdBuffer, "exit")) { //Espera até o usuário digitar "exit"
        clearBuffer(rdCmdBuffer, BUFFER_SIZE); //Limpa o buffer
        typePrompt(); //Imprime o prompt
        readCommand(rdCmdBuffer, BUFFER_SIZE); //Lê os comandos digitados pelo usuário

        cId = fork(); //Cria um processo filho
        if(cId != 0) {
            wait(&cStatus); //Espera o processo filho terminar
        } else {
            /** Deve ser melhorado para suportar um comando e seus argumentos digitados pelo usuário **/
            execv(rdCmdBuffer, argv); //Executa o programa definido no buffer
        }
    }

    exit(0);
}

/** Uma alternativa simples a printf **/
void typePrompt()
{
    char sPrompt[21]; //Aprimorar esta variável

    strcpy(sPrompt, PROMPT);

    fputs(sPrompt, stdout);

    return;
}

/** Limpa o buffer onde são guardados os comandos digitados pelo usuário **/
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
