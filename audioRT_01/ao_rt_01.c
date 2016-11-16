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

#define BUFFER (1024 * 4) //Tamanho do buffer de saída - Não está sendo usado!

#define FREQ 440 //Frequência da senoide

#define NUM_CANAIS 2 //Número de canais de saída
#define BIT_RESOLUCAO 16 //Número de bits de resolução da saída
#define TX_AMOSTRAGEM 44100 //Taxa de amostragem da saída

int pulso(int duracao, int sp_rate) {

}

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
   float teste;

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

   printf("Buffer = %d\n", buf_size);

   for (i = 0; i < format.rate; i++) {

      teste =  ((float) i / format.rate);
      //sample = (int)(10000 * sin(2 * M_PI * freq * ((float) i/format.rate)));

      //printf("Teste %d = %f\n", i, teste);

      /* Gera uma onda quadrada simetrica */
      /*
      if( sample > 0 ) {
         sample = 0x5fff;
      } else {
         sample = 0;
      }
      */

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
      if( teste < 0.05 ) {
         sample = 0x5fff;
      } else if(teste >= 0.15 && teste < 0.25) {
         sample = 0x5fff;
      } else if( teste >= 0.35 && teste < 0.45) {
         sample = 0x5fff;
      } else {
         sample = 0;
      }

      /* Atribui o mesmo sinal é atribuido a ambos os canais */
      buffer[4*i] = buffer[4*i+2] = sample & 0xff;
      buffer[4*i+1] = buffer[4*i+3] = (sample >> 8) & 0xff;
   }

   ao_play(device, buffer, buf_size);

   /* Encerra o drive */
   ao_close(device);

   ao_shutdown();

   return (0);
}
