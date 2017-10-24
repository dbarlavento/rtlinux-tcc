/*
 *	Implementação das funções de inicialização de processos de tempo real
 */

#include "myrt.h"

#include <sched.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>

int setPrioridade( int pri, int pol ) {
	struct sched_param param;
	param.sched_priority = pri;

	if( sched_setscheduler( 0, pol, &param ) == -1 ) {
		return 1;
	} else {
		return 0;
	}
}

void stackPrefault( int tp ) {
	unsigned char pilha[ tp ];
	memset( pilha, 0, tp );
}

int travaMemoria( int tPilha ) {
	if( mlockall( MCL_CURRENT | MCL_FUTURE ) == -1 ) {
		return 1;
	} else {
		stackPrefault( tPilha );
		return 0;
	}
}

void nextShot( int intervalo, struct timespec *t ) {
	t->tv_nsec += intervalo;
	while( t->tv_nsec >= NSEG_POR_SEG ) {
		t->tv_nsec -= NSEG_POR_SEG;
		t->tv_sec++;
	}
}

void normTempo( struct timespec *t ) {
	while( t->tv_nsec >= NSEG_POR_SEG ) {
		t->tv_nsec -= NSEG_POR_SEG;
		t->tv_sec++;
	}
}

int calcDifNseg( struct timespec t1, struct timespec t2 ) {
	int dif = 0;
	dif = NSEG_POR_SEG * (int) ( t2.tv_sec - t1.tv_sec );
	dif += (int) ( t2.tv_nsec - t1.tv_nsec );
	return dif;
}

int calcDifUseg( struct timespec t1, struct timespec t2 ) {
	int dif = 0;
	dif = USEG_POR_SEG * (int) ( t2.tv_sec - t1.tv_sec );
	dif += (int) (  ( t2.tv_nsec - t1.tv_nsec ) /1000 );
	return dif;
}
