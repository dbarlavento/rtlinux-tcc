/*
    Programa configurado com uma política de escalonamento de tempo real
    que cria um processo filho, espera o fim da execução deste e encerra.

    O programa permite utilizar um parâmetro par configurar o valor de sua
    prioridade:

    -p "valor"

    Caso nenhum valor seja passado o valor 50 será assumido como padrão
    Os valores aceitos vão de 0 a 99.
    Caso o valor informado esteja fora do intervalo especificado uma menssagem
    de alerta será exibida e o programa será executado com prioridade no valor
    padrão.

    Este programa foi escrito com o intuito de observar como um processo
    gerado por uma chamada fork herda a prioridade e o tipo de escalonamento
    de seu pai.
*/

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

/*
    Define a prioridade padrão do processo
*/
#define PRIORIDADE_PADRAO (50)

/*
    Define o tamanho da pilha do programa para evitar paginação de memória
    do programa.
 */
#define MAX_SAFE_STACK (8*1024)

/*
    Define a precisão do timer em nanosegundos
*/
#define NSEC_PER_SEC (1000000000)

void stack_prefault(void);

int main(int argc, char* argv[])
{
    char optc = 0;

    struct sched_param confEscalonador; //Contém os parâmetros de escalonamento

    /*
        Lê os parâmetros digitados pelo usuário
    */
    if( argc > 1) {
        while( ( optc = getopt(argc, argv, "p:" ) )
         != -1 ) {
            switch( optc ) {
                case 'p':
                    confEscalonador.sched_priority = atoi(optarg);
                    break;
                default:
                    perror("Parâmetros incorretos.");
                    exit(-1);
            }
        }
    } else {
        confEscalonador.sched_priority = PRIORIDADE_PADRAO;
    }

    /*
        Configura a política de escalonamento do processo
    */
    if(sched_setscheduler(0, SCHED_FIFO, &confEscalonador) == -1) {
        perror("sched_setscheduler - ERRO");
        exit(-1);
    }
}

/*
    Inicializa a pilha do programa com zeros
*/
void stack_prefault(void)
{
    unsigned char dummy[MAX_SAFE_STACK];

    /* Preenche o array dummy com zeros */
    memset(dummy, 0, MAX_SAFE_STACK);
    return;
}
