#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <string.h>

/*
 * Este programa executa uma sucessão de prints na tela do console
 * com o valor de uma variável i, inicialmente igual a 0, em um intervalo
 * de tempo definido pelo usuário.
 * Esta versão utiliza a política de escalonamento SCHED_FIFO.
 * O primeiro parâmetro equivale a prioridade do processo.
 * O segundo parâmetro equivale ao nome exibido nos printfs.
 * O terceiro parâmetro equivale ao período de ativação do processo.
 */

/* Define a precisão do timer em nanosegundos */
#define NSEC_PER_SEC (1000000000)

int main(int argc, char* argv[])
{
    int appNum;
    int i = 0;
    int interval; //Valor em nanosegundos
    struct sched_param param;
    struct timespec t;
    struct timespec tbuf1, tbuf2;

    /* Criar um interpretador melhor, esta solução é muito ruim */

    /* Configura a prioridade do processo o padrão é 1*/
    param.sched_priority = (argc > 1) ? atoi(argv[1]) : 1;

    /* Configura o nome do processo */
    appNum = (argc > 2) ? atoi(argv[2]) : 100;

    /* Configura o intervalo de tempo entre prints  o padrão é 1s*/
    interval = (argc > 3) ? atoi(argv[3]) : 1000000000;

    /* Configura a política de escalonamento do processo */
    if(sched_setscheduler(0, SCHED_FIFO, &param) == -1)
    {
        perror("sched_setscheduler failed");
        exit(-1);
    }

    /* Lê o valor de tempo corrente */
    clock_gettime(CLOCK_MONOTONIC ,&t);

    /* Inicia após 1s */
    t.tv_sec++;

    for(i ;; i++)
    {
        clock_gettime(CLOCK_MONOTONIC ,&tbuf1);
        printf("\nNanosegundos antes de dormir: %ld", tbuf1.tv_nsec);
        /*  */
        clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

        clock_gettime(CLOCK_MONOTONIC ,&tbuf2);
        printf("\nNanosegundos depois de dormir: %ld", tbuf2.tv_nsec);

        if( (tbuf2.tv_nsec - tbuf1.tv_nsec) <= interval) {
            printf("\nApp %d - Shot nº: %d\n", appNum, i);
        } else {
            printf("\nApp %d - ERRO\n", appNum);
        }

        t.tv_nsec += interval;

        while (t.tv_nsec >= NSEC_PER_SEC)
        {
            t.tv_nsec -= NSEC_PER_SEC;
            t.tv_sec++;
        }
    }
}
