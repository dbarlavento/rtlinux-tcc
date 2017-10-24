/*
 *	Programa que imprime uma mensagem a cada intervalo de tempo aleatório.
 *	Este mostra como inicializar uma thread para que está faça, de forma
 *	adequada, dos recursos de tempo real disponibilizados pelo patch preempt-rt.
 * Nesta versão as rotinas de inicialização foram encapsuladas em funções.
 */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>

#include "initRt.h"

/* Define a prioridade da thread principal do programa. */
#define PRIORIDADE (90)

/* 
 *	Define o tamanho máximo da pilha do programa que deve ser bloqueada
 *	proporcionando um acesso seguro sem que ocorra paginação.
 */
#define MAX_PROG_PILHA (8*1024)

/* Define quantos nano segundos existem em um segundo. */
#define NSEG_POR_SEG (1000000000)

int main( int argc, char* argv[]) {
	unsigned int i = 0;
	unsigned int r = 0;
	struct timespec tempo;

	if( setPrioridade( PRIORIDADE, SCHED_FIFO ) == 1 ) {
		perror( "Falha na configuração do escalonador" );
		exit( -1 );
	}

	/* Trava as posições de memória */
	if( travaMemoria( MAX_PROG_PILHA ) == 1 ) {
		perror( "Falha na alocação de memória" );
		exit( -2 );
	} 

	srand( time( NULL ) );

	clock_gettime( CLOCK_MONOTONIC, &tempo );
	r = ( ( unsigned ) rand() ) % NSEG_POR_SEG;
	tempo.tv_nsec += r;

	for( i; ; i++) {
		clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME, &tempo, NULL );
	
		r = ( ( unsigned ) rand() ) % NSEG_POR_SEG;
		tempo.tv_nsec += r;

		printf( "\nEsperou: %d\n", ( r ) );
		printf( "Segundos: %ld\n", (long) tempo.tv_sec );
		printf( "NanoSec: %ld\n", (long) tempo.tv_nsec );
		
		while( tempo.tv_nsec >= NSEG_POR_SEG ) {
			tempo.tv_nsec -= NSEG_POR_SEG;
			tempo.tv_sec++;
		}			
 	}
}
 
