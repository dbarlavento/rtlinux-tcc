#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/*
	Exibe quantos ciclos de clock foram gastos na inicialização do programa
	e quantos foram gastos no processamento até o final do mesmo.
*/

int main() {
	//Tempo no inicio do programa
	clock_t start_t = clock();

	bool flag = true;
	char quit = ' ';
	int cInput = 0;
	
	//Tempo no fim do programa
	clock_t end_t;
	
	//Tempo total de execução
	double total_t;
	
	printf("\nPrograma iniciado em: %ld\n", start_t);
	
	while( flag == true ) {
		quit = getchar();		
		putchar(quit);
		if(quit == 'q') {
			flag = false;
		}
	}
	
	end_t = clock();
	printf("\nPrograma terminado em: %ld\n", end_t);
	
	total_t = (double) ((end_t - start_t) / CLOCKS_PER_SEC);
	printf("\nDuração total: %f\n", total_t);
	
	printf("\nClocks por segundo: %ld\n", CLOCKS_PER_SEC);
	
	return 0;
}
