#include <time.h>

#ifndef MYRT_H
#define MYRT_H 

/* Número de nanosegundos por segundos */
#define NSEG_POR_SEG (1000000000)
#define USEG_POR_SEG (1000000)

/*
 * Cabeçalho das funções de inicialização de threds de tempo real
 */

/*
 *	Configura a prioridade de um processo e define a política de escalonamento
 * Retorna 1 em caso de falha e 0 para sucesso
 * Para processos de tempo real "pri" deve ter um valor entre 0 e 99
 * O parâmetro "pol" deve ter um entre os cinco valores de finidos para as 
 * políticas de escalonamento definidas em sched.h
 */
int setPrioridade( int pri, int pol );

/*
 *	Inicializa a pilha do programa com zeros
 * O parâmetro tp define o tamanho da pilha
 */
void stackPrefalt( int tp );

/*
 *	Garante que a porção de memoria do processo não será alterada nem paginada
 *	Inicializa as posições de memória com 0
 * Retorna 1 em caso de falha e 0 para sucesso
 * O parâmetro tPilha deve ter o tamanho da pilha do processo que se deseja 
 * bloquear. No linux a pilha de um processo normalmente tem 8MB de tamnho.
 */
int travaMemoria( int tPilha );

/*
 * Calcula o tempo após um intervalo definido e normaliza os valores de tempo
 */
void nextShot( int intervalo, struct timespec *t );

/*
 * Normaliza os valores de tempo
 */
void normTempo( struct timespec *t );

/*
 * Calcula a diferensa de tempo entre dois momentos específicos em nanosegundos
 * Retorna t2 - t1
 */
int calcDifNseg( struct timespec t1, struct timespec t2 );  

/*
 * Calcula a diferensa de tempo entre dois momentos específicos em microsegundos
 * Retorna t2 - t1
 */
int calcDifUseg( struct timespec t1, struct timespec t2 );  

#endif
