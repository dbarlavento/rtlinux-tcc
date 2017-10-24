/*
 *	Teste da função clock_gettime()
 * Mede o overhead da função e apresenta o resultado como um histograma 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_MED (1000)
#define MAX_LAT (500)

int main( int argc, char** argv ) {
	int i = 0;
	long lat[MAX_LAT + 1];
	struct timespec t1;
	struct timespec t2;

	for( i = 0; i < ( MAX_LAT + 1 ); i++ ) {
		lat[i] = 0;
	}
	
	for( i = 0; i < NUM_MED; i++ ) {

		clock_gettime( CLOCK_MONOTONIC, &t1 );
		clock_gettime( CLOCK_MONOTONIC, &t2 );

		t2.tv_sec -= t1.tv_sec;
		t2.tv_nsec -= t1.tv_nsec;
	
		if( t2.tv_sec == 0 && t2.tv_nsec < 500 && t2.tv_nsec >= 0 ) {
			lat[t2.tv_nsec]++;
		}
		else {
			lat[MAX_LAT]++;
		}

	}

	printf( "#Histograma\n" );
	
	for( i = 0; i < MAX_LAT; i++ ) {
		printf( "%03d %05d\n", i, lat[i] );
	}	

	printf( ">%03d %05d\n", MAX_LAT, lat[MAX_LAT] );
	
	exit(0);
}
