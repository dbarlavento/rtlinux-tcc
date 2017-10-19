
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_C8051F330_Register_Enums.h>                // SFR declarations

//-----------------------------------------------------------------------------
// Definições
//-----------------------------------------------------------------------------
#define SYSCLK      24500000	//Clock do sistema em Hz
#define BAUDRATE	9600		//Velocidade da conexão serial em bps

SBIT(LED, SFR_P1, 3);	//Define o LED como sendo o P1.3
SBIT(BT, SFR_P0, 7);	//Define o botão como sendo o P0.7

//INTERRUPT_PROTO(INT0_INTERRUPT, INT0_IRQn);

void clockInit(void);	//Inicializa o clock do sistema
void portInit(void);	//Inicializa os pinos de I/O
void serialInit(void);	//Inicializa a porta serial
void interrupInit(void);	//Inicializa as opções de interrupção

//-----------------------------------------------------------------------------
// Rotina Principal
//-----------------------------------------------------------------------------
int main (void)
{
	clockInit();
	portInit();
	serialInit();
	//interrupInit();

	LED = 0;

	while (1) {
		if(BT == 0) {
			if(BT == 0) {
				LED = ~LED;
			}
		}
	}
}

//-----------------------------------------------------------------------------
// Rotinas De Inicialização
//-----------------------------------------------------------------------------

//Inicialização do clock
void clockInit(void) {
	//Define o divisor do clock para 1
	OSCICN |= 0x03;

	//Define que o MCU deve ser reiniciado em caso de ausência de clock
	RSTSRC = 0x04;
}

//Inicializa os pinos de I/O
void portInit(void) {
	//Configura TX e RX para os pinos P0.4 e P0.5 respectivamente
	XBR0 = 0x01;
	//Habilita o crossbar
	XBR1 = 0x40;

	//Configura os pinos de P0 como entrada digital
	P0MDIN = 0xFF;
	//Configura o pino P0.4 (TX) como saída "push-pull"
	P0MDOUT |= 0x10;

	//Configura os pinos de P1 como entrada digital
	P1MDIN = 0xFF;
	//Configura o pino P1.3 como saída "push-pull"
	P1MDOUT |= 0x08;
}

//Inicializa a porta serial
void serialInit(void) {
	/*
	 * Habilita a recepção de dados pela canal serial e configura o modo
	 * como 8 bits e baudrate variável.
	 */
	//SCON0 = 0x10;

	/*
	 * Este bloco define as configurações de T1 a partir dos valores das
	 * constantes SYSCLK e BAUDRATE.
	 */
	/*
	if (SYSCLK/BAUDRATE/2/256 < 1) {
		TH1 = -(SYSCLK/BAUDRATE/2);
		CKCON &= ~0x0B;                  // T1M = 1; SCA1:0 = xx
		CKCON |=  0x08;
	} else if (SYSCLK/BAUDRATE/2/256 < 4) {
		TH1 = -(SYSCLK/BAUDRATE/2/4);
		CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 01
		CKCON |=  0x01;
	} else if (SYSCLK/BAUDRATE/2/256 < 12) {
		TH1 = -(SYSCLK/BAUDRATE/2/12);
		CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 00
	} else {
		TH1 = -(SYSCLK/BAUDRATE/2/48);
		CKCON &= ~0x0B;                  // T1M = 0; SCA1:0 = 10
		CKCON |=  0x02;
	}
	*/
}

//Inicializa as interrupções necessárias ao sistema
void interrupInit(void) {
	//Habilita a interrupção INT0
	IE = 0x81;

	//Configura INT0 para o pino 0.7 e para ser ativo quando o nível for baixo
	IT01CF = 0x07;
}

/*
INTERRUPT(INT0_INTERRUPT, INT0_IRQn) {
	LED = ~LED;
}
*/



