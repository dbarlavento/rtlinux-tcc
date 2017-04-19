#include <stdio.h>
#include <stdlib.h>
#include <alsa/asoundlib.h>

int main(int argc, char *argv[]) {

	unsigned char *data;
   int pcmreturn, l1, l2;
   short s1, s2;
   int frames;

	/* Parametros de configuração da placa de som */
	int rate = 44100; /* Sample rate */
   int exact_rate;   /* Sample rate returned by */
                     /* snd_pcm_hw_params_set_rate_near */ 
   int dir;          /* exact_rate == rate --> dir = 0 */
                     /* exact_rate < rate  --> dir = -1 */
                     /* exact_rate > rate  --> dir = 1 */
   int periods = 2;  /* Number of periods */
   snd_pcm_uframes_t periodsize = 8192; /* Periodsize (bytes) */

	/* Manipulador para dispositivos PCM */
	snd_pcm_t *pManipuladorPcm;
	
	/* Reprodutor de stream */
	snd_pcm_stream_t stream = SND_PCM_STREAM_PLAYBACK;
	
	/* Estrutura que contem as informações sobre o hardware */
	snd_pcm_hw_params_t *hwparams;
	
	/* Nome do dispositivo PCM */
	char *pNomePcm = strdup("plughw:0,0");
	
	/* ???? */
	snd_pcm_hw_params_alloca(&hwparams);
	
	/* Abre a interface PCM */
	if(snd_pcm_open(&pManipuladorPcm, pNomePcm, stream, 0) < 0) {
      fprintf(stderr, "Error opening PCM device %s\n", pNomePcm);
      return(-1);
   }
    
   /* Configura a placa de som com os parametros pretendidos */
   if(snd_pcm_hw_params_any(pManipuladorPcm, hwparams) < 0) {
     fprintf(stderr, "Can not configure this PCM device.\n");
     return(-1);
   }
   
   /* Set access type. This can be either    */
    /* SND_PCM_ACCESS_RW_INTERLEAVED or       */
    /* SND_PCM_ACCESS_RW_NONINTERLEAVED.      */
    /* There are also access types for MMAPed */
    /* access, but this is beyond the scope   */
    /* of this introduction.                  */
    if (snd_pcm_hw_params_set_access(pManipuladorPcm, hwparams, SND_PCM_ACCESS_RW_INTERLEAVED) < 0) {
      fprintf(stderr, "Error setting access.\n");
      return(-1);
    }
  
    /* Set sample format */
    if (snd_pcm_hw_params_set_format(pManipuladorPcm, hwparams, SND_PCM_FORMAT_S16_LE) < 0) {
      fprintf(stderr, "Error setting format.\n");
      return(-1);
    }

    /* Set sample rate. If the exact rate is not supported */
    /* by the hardware, use nearest possible rate.         */ 
    exact_rate = rate;
    if (snd_pcm_hw_params_set_rate_near(pManipuladorPcm, hwparams, &exact_rate, 0) < 0) {
      fprintf(stderr, "Error setting rate.\n");
      return(-1);
    }
    
    if (rate != exact_rate) {
      fprintf(stderr, "The rate %d Hz is not supported by your hardware.\n ==> Using %d Hz instead.\n", rate, exact_rate);
    }

    /* Set number of channels */
    if (snd_pcm_hw_params_set_channels(pManipuladorPcm, hwparams, 2) < 0) {
      fprintf(stderr, "Error setting channels.\n");
      return(-1);
    }

    /* Set number of periods. Periods used to be called fragments. */ 
    if (snd_pcm_hw_params_set_periods(pManipuladorPcm, hwparams, periods, 0) < 0) {
      fprintf(stderr, "Error setting periods.\n");
      return(-1);
    }
    
    if (snd_pcm_hw_params_set_buffer_size(pManipuladorPcm, hwparams, (periodsize * periods)>>2) < 0) {
      fprintf(stderr, "Error setting buffersize.\n");
      return(-1);
    }
    
    if (snd_pcm_hw_params(pManipuladorPcm, hwparams) < 0) {
      fprintf(stderr, "Error setting HW params.\n");
      return(-1);
    }
    
    data = (unsigned char *)malloc(periodsize);
    frames = periodsize >> 2;
    for(l1 = 0; l1 < 100; l1++) {
      for(l2 = 0; l2 < frames; l2++) {
        s1 = (l2 % 128) * 100 - 5000;  
        s2 = (l2 % 256) * 100 - 5000;  
        data[4*l2] = (unsigned char)s1;
        data[4*l2+1] = s1 >> 8;
        data[4*l2+2] = (unsigned char)s2;
        data[4*l2+3] = s2 >> 8;
      }
      while ((pcmreturn = snd_pcm_writei(pManipuladorPcm, data, frames)) < 0) {
        snd_pcm_prepare(pManipuladorPcm);
        fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
      }
    }
}
