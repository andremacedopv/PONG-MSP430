#include <msp430.h>
#include <stdlib.h>
#include "joystick.h"

//******************************************************************************************************************************************
void confiAnalogPins(){
	// Eixo do analogco (Player 1: P1.3, Player 2: P1.4)
	P1DIR &= ~(BIT3|BIT4);
    P1SEL0 = (BIT3|BIT4);
    P1SEL1 = (BIT3|BIT4);
    // Botao do Analogico (Player 1: P3.6, Player 2: P3.7)
    P3DIR &= ~(BIT6|BIT7);
    P3REN |= (BIT6|BIT7);
	P3OUT |= (BIT6|BIT7);
}
//******************************************************************************************************************************************
uint16_t readAnalog(uint8_t player){
	ADC12CTL0 &= ~ADC12ENC;                                  // desabilitiar conversao
    ADC12CTL0 |= ADC12SHT0_0 | ADC12ON;                      //  hablitar conversor
    ADC12CTL1 = ADC12SHS_0 | ADC12SHP | ADC12CONSEQ_0;       // single-channel, sngle-conversion
    ADC12CTL2 = ADC12RES_2;                                  // modo 2 bits
    ADC12CTL3 &= ~ADC12CSTARTADD0;

    if(player == PLAYER1){
    	ADC12MCTL0 =  0x0003;
	}
	else if(player == PLAYER2){
		ADC12MCTL0 =  0x0004;
	}

	// Criar pulso
    ADC12CTL0 |= ADC12ENC;
    ADC12CTL0 &= ~ADC12SC;
    ADC12CTL0 |= ADC12SC;

    while(!(ADC12IFGR0 & ADC12IFG0));
    return ADC12MEM0;                                        // retornar valor lido
}
//******************************************************************************************************************************************
TPaddle* updatePaddles(){
	uint16_t read;
	TPaddle *pad = malloc(sizeof(TPaddle)*2);
	//Player 1
	read = readAnalog(PLAYER1);  // ler valor do analógico
	// subir
	if(read >= 2800){
		pad[PLAYER1] = gameState.paddle[PLAYER1] - 1;
	}
	// descer
	else if(read < 1400){
		pad[PLAYER1] = gameState.paddle[PLAYER1] + 1;
	}
	// manter posição
	else{
		pad[PLAYER1] = gameState.paddle[PLAYER1];
	}

	//Player 2
	read = readAnalog(PLAYER2); // ler valor do analógico
	//subir 
	if(read >= 2800){
		pad[PLAYER2] = gameState.paddle[PLAYER2] - 1;
	}
	// descer
	else if(read < 1400){
		pad[PLAYER2] = gameState.paddle[PLAYER2] + 1;
	}
	// manter
	else{
		pad[PLAYER2] = gameState.paddle[PLAYER2];
	}

	return pad; // retornar novas posições
}