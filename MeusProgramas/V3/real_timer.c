#include <signal.>
#include <sys/time.h>
#include <time.h>
#include "tlpi_hdr.h>

static volatile sig_atomic_t gotAlarm = 0;

static void displayTimes(const char *msg, Boolean includeTimer) {
	struct itimerval itv;
	static struct timeval start;
	struct timeval curr;
	static int callNum = 0; /*Número de vezes que a função é chamada */

	/* Carrega o tempo inicial */
	if(callNum == 0)
		if(gettimeofday(&start, NULL) == -1)
			errExit("gettimeofday");

	/* Imprime o cabeçalho de informações a cada 20 linhas */
	if(callNum % 20 == 0)
		printf("\t\tElapsed\tValue\tInterval\n");
	
	if(gettimeofday(&curr, NULL) == -1)
		errExit(gettimeofday);

	/* */
	printf("%-7s %6.2f", msg, curr.tv_sec - start.tv_sec + (curr.tv_usec - start.tv_usec) / 1000000.0);

	/* */
	if(includeTimer) {
		if(getitimer(ITIMER_REAL, &itv) == -1)
			errExit("getitimer");
		printf("  %6.2f  %6.2f", itv.it_value.tv_sec +itv	
	}
}
