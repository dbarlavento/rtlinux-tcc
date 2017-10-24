/*
 *	Teste da função clock_gettime()
 * Mede o overhead da função e apresenta o resultado como um histograma 
 * Esta versão executa como um processo de tempo real
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>

#include "myrt.h"

#define NUM_MED (1000) //Número de medições
#define MIN_LAT (0) //Latência mínima em microsegundos
#define MAX_LAT (100) //Latência máxima em microsegundos

/*
 * Define o tamanho do array que guardará os valores do histograma
 * O 3 corresponde ao valor da latência máxiam,
 * as medições menores que MIN_LAT e
 * as medições maiores que MAX_LAT.
 */
#define TAM_ARRAY ( 3 + MAX_LAT - MIN_LAT )

#define PRIORIDADE (90)
#define MAX_PROG_PILHA (8*1024)

#define INTERVALO (1000000) //Intervalo de 1ms

int main( int argc, char** argv ) {
	int i = 0;
	int dif = 0;
	int lat[ TAM_ARRAY ];
	
	struct timespec t1;
	struct timespec t2;

	/* Inicializa processo como de tempo real */
	
	if( setPrioridade( PRIORIDADE, SCHED_FIFO ) == 1  ) {
		perror( "Falha na configuração do escalonador" );
		exit( -1 );
	}
	if( travaMemoria( MAX_PROG_PILHA ) == 1 ) {
		perror( "Falha na alocação de memória" );
		exit( -2 );
	}

	for( i = 0; i < TAM_ARRAY; i++ ) {
		lat[i] = 0;
	}
	
	/*
 	 * Mede a latência dentro do intervalo definido.
	 * Valores menores que a latência mínima são colocados na posição 0 do array
	 * Valores maiores que a latência máxima são colocados na ultima posição do array
	 */
	/*
	for( i = 0; i < NUM_MED; i++ ) {

		clock_gettime( CLOCK_MONOTONIC, &t1 );
		clock_gettime( CLOCK_MONOTONIC, &t2 );

		t2.tv_sec -= t1.tv_sec;
		t2.tv_nsec -= t1.tv_nsec;
	
		if( t2.tv_sec == 0 && t2.tv_nsec <= MAX_LAT && t2.tv_nsec >= MIN_LAT ) {
			lat[1 + t2.tv_nsec - MIN_LAT]++;
		}
		else {
			printf( "\nMaior que MAX_LAT: i = %d\n", i );
			if( t2.tv_sec > 0 || t2.tv_nsec > MAX_LAT ) {
				lat[TAM_ARRAY - 1]++;
			} else {
				lat[0]++;
			}
		}

	}
	*/

	/*
	 * Mede a latência dentro de um intervalo definido usando o algoritmo de cyclictest
  	 */
	printf( "\n Inicio:\n" );
	clock_gettime( CLOCK_MONOTONIC, &t1 );
	t2 = t1;
	nextShot( INTERVALO, &t2 );
	
	for( i = 0; i < NUM_MED; i++ ) {
		clock_nanosleep( CLOCK_MONOTONIC, TIMER_ABSTIME, &t2, NULL );
		clock_gettime( CLOCK_MONOTONIC, &t1 );
		
		dif = calcDifUseg( t2, t1 );
		
		if( dif <= MAX_LAT && dif >= MIN_LAT ) {
			lat[ 1 + dif - MIN_LAT ]++;
		}	
		else {
			if( dif > MAX_LAT ) {
				lat[ TAM_ARRAY - 1 ]++;
			} else {
				lat[ 0 ]++;
			}
		}

		nextShot( INTERVALO, &t2 );
	}
	
	printf( "#Histograma\n" );

	printf( "<%06d %06d\n", MIN_LAT, lat[0] );
	
	for( i = 0; i < ( TAM_ARRAY - 2 ) ; i++ ) {
		printf( "%06d %06d\n", ( i + MIN_LAT ), lat[i + 1] );
	}	

	printf( ">%06d %06d\n", MAX_LAT, lat[TAM_ARRAY - 1] );
	
	exit(0);
}
