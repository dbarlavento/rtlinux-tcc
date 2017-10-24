/*
 *	Teste da função rand()
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main( int argc, char** argv ) {
	time_t t;
	struct timespec rt;
	unsigned int r1 = 0;
	unsigned int r2 = 0;

	printf( "\nGera um número aleatório de 0 a %d.\n", RAND_MAX );
	
	srand( ( unsigned ) time( &t ) );
	
	clock_gettime( CLOCK_MONOTONIC, &rt );

	printf( "\nNSEC: %ld.\n", rt.tv_nsec );

	r1 = ( unsigned ) rand();
	r2 = r1 % 1000000000;
	rt.tv_nsec += r2;

	printf( "\nO número gerado é: %d.\n", r1 );
	printf( "Um número entre 0 e 10^9 - 1: %d.\n", r2 );
	printf( "NSEC + r: %ld.\n", rt.tv_nsec );

	exit(0);
}
