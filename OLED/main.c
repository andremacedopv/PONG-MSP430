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
  //volatile uint32_t i;

  WDTCTL = WDTPW | WDTHOLD;
  PM5CTL0 &= ~LOCKLPM5;

  ssd1306Init();

  fillDisplay(0x00);

  confiAnalogPins();

  printInit();
  while((P3IN & BIT6) && (P3IN & BIT7));
  delay(5000);

  fillDisplay(0x00);

  gameInit();
  printGame(gameState);

  /*int n=1, m=-1;
  uint8_t i;
  int j=12;
  i = 12;*/
  TPaddle *pad;

  while (1){

      pad = updatePaddles();
      updateGameState(pad[PLAYER1], pad[PLAYER2]);
      printGame(gameState);
      free(pad);
      /*if(i > 22){
          n = -1;
          i += n;
      }
      else if(i < 1){
          n = 1;
          i = 1 + n;
      }
      else{
          i += n;
      }
      if(j > 22){
          m = -1;
          j += m;
      }
      else if(j < 1){
          m = 1;
          j = 1 + m;
      }
      else{
          j += m;
      }*/
  }

}
