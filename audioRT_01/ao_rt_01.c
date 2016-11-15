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
 * Versão atual: 1
 * Data: 15/11/2016
 *
 * 15-11-2016: Apenas gera uma onda senoidal de 440Hz é gerada por um segundo.
 *
 * Nota: A compilação deste programa exige os parâmetros do gcc:
 * -lao
 * -ldl
 * -lm
 * -lrt (não na versão 1)
 */

#define BUFFER 4096

#define FREQ 440 //Frequência da senoide

#define NUM_CANAIS 2 //Número de canais de saída
#define BIT_RESOLUCAO 16 //Número de bits de resolução da saída
#define TX_AMOSTRAGEM 44100 //Taxa de amostragem da saída

int main(int argc, char* argv[])
{
   ao_device *device;
   ao_sample_format format;
   int default_driver;
   char *buffer;
   int buf_size;
   int sample;
   float freq = FREQ;
   int i;

   fprintf(stderr, "Áudio de tempo real - v.1\n");

   /* Inicialização do dispositivo de Áudio */

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
      fprintf(stderr, "Erro ao abrir o drive.\n");
      return 1;
   }

   /* -- Play some stuff -- */
   buf_size = format.bits/8 * format.channels * format.rate;
   buffer = calloc(buf_size, sizeof(char));

   for (i = 0; i < format.rate; i++) {
      sample = (int)(0.75 * 32768.0 *
                     sin(2 * M_PI * freq * ((float) i/format.rate)));

      /* Atribui o mesmo sinal a ambos os canais */
      buffer[4*i] = buffer[4*i+2] = sample & 0xff;
      buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
   }

   ao_play(device, buffer, buf_size);

   /* Encerra o drive */
   ao_close(device);

   ao_shutdown();

   return (0);
}
