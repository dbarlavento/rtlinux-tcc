/*
 *	Programa que cria algumas threads que imprimem seu próprio número ID
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

/* Número de threads que serão criadas */
#define NUM_THREADS (5)

/* 
 * Função que será executada pelas threads.
 * Esta função imprime os valores de ID da thread corrente
 * retornados por getid() e pthread_self(). 
 */
void *impId( void * args ) {
	int idSelf, idKernel;
	
	idSelf = (int) pthread_self();
	idKernel = (int) syscall( SYS_gettid );

	printf( "\nID - Thread: %ld\n", idSelf);
	printf( "ID - Kernel: %d\n\n", idKernel);

	return(NULL);		
}

int main( int argc, char **argv ){
	pthread_t threads[NUM_THREADS];
	int i = 0;

	for( i; i < NUM_THREADS; i++ ) {
		pthread_create( &threads[i], NULL, impId, NULL );
	}

	printf( "\nMain: Esperando o fim das threads ...\n" );

	i = 0;
	for( i; i < NUM_THREADS; i++ ) {
		pthread_join( threads[i], NULL );
	}
	return 0;
}
