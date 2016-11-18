#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sched.h>
#include <sys/mman.h>
#include <ao/ao.h>

/*
 * Programa que gera uma onda senoidal de frequência e duração definidas pelo usuário.
 * Autor: Daniel Barlavento Gomes
 * Versão atual: 3
 * Data: 15/11/2016
 * Última atualização: 16/11/2016
 *
 * 15-11-2016: Apenas gera uma onda senoidal de 440Hz é gerada por um segundo.
 *
 * 16-11-2016: A onda de saída agora é uma onda quadrada com nível baixo igual a zero
 *
 * 16-11-2016: Adicionados mais dois modos de saída, pulso e 8 bits.
 *
 * Nota: A compilação deste programa exige os parâmetros do gcc:
 * -lao
 * -ldl
 * -lm
 * -lrt (não na versão RT)
 */

#define TAM_PILHA (1024 * 4) //Tamanho da pilha do programa

#define PRIORIDADE 99 //Prioridade do processo

#define NSEG_POR_SEG (1000000000) //Resolução do timer

#define FREQ 440 //Frequência da senoide, para saidas ciclicas

#define NUM_CANAIS 2 //Número de canais de saída
#define BIT_RESOLUCAO 16 //Número de bits de resolução da saída
#define TX_AMOSTRAGEM 44100 //Taxa de amostragem da saída

/* Inicializa a pilha com zeros  */
void pilha_ini(void)
{
   unsigned char pilha[TAM_PILHA];
   memset(pilha, 0, TAM_PILHA);
   return;
}


int main(int argc, char* argv[])
{
   /* Variáveis para gestão de tempo real */
   struct timespec t;
   struct sched_param param;
   int interval = 1000000000; /* 10ms */

   /* Variáveis para gestão do drive de áudio */
   ao_device *device;
   ao_sample_format format;
   int default_driver;
   char *buffer;
   int buf_size;
   int sample;
   float freq = FREQ;
   int i;
   float teste;

   /* Splash! */
   fprintf(stderr, "Áudio de tempo real - v.4\n");

   /* Configura o programa como de tempo real */
   param.sched_priority = PRIORIDADE;
   if(sched_setscheduler(0, SCHED_FIFO, &param) == -1) {
      perror("Falha ao configurar o escalonador.");
      exit(-1);
   }

   /* Bloqueia os endereços de memória do processo na RAM */
   if(mlockall(MCL_CURRENT|MCL_FUTURE) == -1) {
      perror("Falha ao bloquear memória.");
      exit(-2);
   }

   /* Inicializa a pilha do programa */
   pilha_ini();

   /* Inicialização do dispositivo de áudio */
   ao_initialize();

   /* Configuração do drive de áudio */
   default_driver = ao_default_driver_id();
   memset(&format, 0, sizeof(format));
   format.bits = BIT_RESOLUCAO;
   format.channels = NUM_CANAIS;
   format.rate = TX_AMOSTRAGEM;
   format.byte_format = AO_FMT_LITTLE;

   /* Abre o drive de áudio */
   device = ao_open_live(default_driver, &format, NULL);
   if (device == NULL) {
      perror("Erro ao abrir o drive de áudio.");
      exit(-3);
   }

   /* Configura o tamanho do buffer */
   buf_size = format.bits/8 * format.channels * format.rate;
   buffer = calloc(buf_size, sizeof(char));

   /* Geração da saída */
   for (i = 0; i < format.rate; i++) {

      //teste =  ((float) i / format.rate);
      sample = (int)(10000 * sin(2 * M_PI * freq * ((float) i/format.rate)));

      //printf("Teste %d = %f\n", i, teste);

      /* Gera uma onda quadrada simetrica */
      if( sample > 0 ) {
         sample = 0x5fff;
      } else {
         sample = 0;
      }

      /* Gera um pulso de 100ms */
      /*
      if( teste < 0.1 ) {
         sample = 0x5fff;
      } else {
         sample = 0;
      }
      */


      /* Gera uma sequencia de pulsos (10011001), cada bit tem duração de 50ms */
      /* ESTA É UMA FORMA MUITO INEFICIENTE DE FAZER ISSO!!! */
      /*
      if( teste < 0.05 ) {
         sample = 0x5fff;
      } else if(teste >= 0.15 && teste < 0.25) {
         sample = 0x5fff;
      } else if( teste >= 0.35 && teste < 0.45) {
         sample = 0x5fff;
      } else {
         sample = 0;
      }
      */

      /* Atribui o mesmo sinal a ambos os canais */
      buffer[4*i] = buffer[4*i+2] = sample & 0xff;
      buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
   }

   /* Inicia a estrututura t com o tempo do sistema */
   clock_gettime(CLOCK_MONOTONIC ,&t);

   /* Loop principal */
   while(1) {
      /* Espera pela próxima execução */
      clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);

      /* Executa o pulso */
      ao_play(device, buffer, buf_size);

      /* calcula o tempo da próxima execução */
      t.tv_nsec += interval;

      while (t.tv_nsec >= NSEG_POR_SEG) {
         t.tv_nsec -= NSEG_POR_SEG;
         t.tv_sec++;
      }
   }

   /* Encerra o drive */
   ao_close(device);

   ao_shutdown();

   return (0);
}
