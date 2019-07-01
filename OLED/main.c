#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "oled_lib.h"
#include "pong.h"
#include "joystick.h"

void delay(uint16_t time_us){
    TA2CTL = TASSEL__SMCLK | MC__UP | TACLR;
    TA2CCR0 = time_us;
    while(!(TA2CCTL0 & CCIFG));
    TA2CCTL0 &= ~CCIFG;
}

int main(void) {

  WDTCTL = WDTPW | WDTHOLD;
  PM5CTL0 &= ~LOCKLPM5;

  // iniciar OLED e esvaziar display
  ssd1306Init();
  fillDisplay(0x00);

  // configurar pinos do analógico
  confiAnalogPins();

  TPaddle *pad;
  while(1){
      // esperar jogador inciar o jogo
      printInit();
      while((P3IN & BIT6) && (P3IN & BIT7));
      delay(5000);

      fillDisplay(0x00);  // limpar display

      // execução do jogo
      gameInit();                                         // iniciar jogo
      printGame(gameState);
      // continuar execução até analógico ser apertado
      while ((P3IN & BIT6) && (P3IN & BIT7)){

          pad = updatePaddles();                          // ler analógicos
          updateGameState(pad[PLAYER1], pad[PLAYER2]);    // atualizar jogo
          printGame(gameState);                           // imprimir jogo
          free(pad);

      }

      // terminar jogo e esperar pressionamento do analógico para iniciar um novo
      endGame();
      while((P3IN & BIT6) && (P3IN & BIT7));
      delay(5000);
      fillDisplay(0x00);
  }

  return 0;
}
