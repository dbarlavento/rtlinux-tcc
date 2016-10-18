#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>

/*
 * Este programa executa uma sucessão de prints na tela do console
 * com o valor de uma variável i, inicialmente igual a 0, a cada 100ms
 */ 

/*	Define a prioridade do processo */
#define MY_PRIORITY (90) /* we use 49 as the PRREMPT_RT use 50
                            as the priority of kernel tasklets
                            and interrupt handler by default */

/* Define o tamanho da pilha do programa para evitar paginação de memória
 *  do programa  */
#define MAX_SAFE_STACK (8*1024) /* The maximum stack size which is
                                   guaranteed safe to access without
                                   faulting */

/* Define a precisão do timer em nanosegundos  */
#define NSEC_PER_SEC (1000000000) /* The number of nsecs per sec. */

/* Inicializa a pilha com zeros  */
void stack_prefault(void) {

	unsigned char dummy[MAX_SAFE_STACK];

	/* Preenche o array dummy com zeros */
   memset(dummy, 0, MAX_SAFE_STACK);
   return;
}

int main(int argc, char* argv[])
{
	
	clock_t clock_ini = clock();
	clock_t clock_fim;
	unsigned int i = 0;
	int num_shots = 0;
   struct timespec t;
   struct sched_param param;
   int interval = 100000000; /* 100ms */

   /* Declare ourself as a real time task */

   param.sched_priority = MY_PRIORITY;
   if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
     	perror("sched_setscheduler failed");
        exit(-1);
   }

   /* Lock memory */

   if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
   	perror("mlockall failed");
      exit(-2);
   }

   /* Pre-fault our stack */

   stack_prefault();

   clock_gettime(CLOCK_MONOTONIC ,&t);
   
	/* start after one second */
   t.tv_sec++;

	printf("inicio do programa: %ld", clock_ini);	
	
   while(num_shots < 100) {
   	/* wait until next shot */
   	clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

      /* do the stuff */
		i = i + (interval / 1000000);
		printf("\nshot: %dms\n", i);
      
		/* calculate next shot */
		t.tv_nsec += interval;

      while (t.tv_nsec >= NSEC_PER_SEC) {
			printf("executou o segundo while, t.tv_nsec: %ld", t.tv_nsec);
      	t.tv_nsec -= NSEC_PER_SEC;
         t.tv_sec++;
      }

		num_shots += 1;
	}
	clock_fim = clock();

	printf("valor final de i = %d\n", i);

	printf("Fim do programa: %ld\n", clock_fim);

}
