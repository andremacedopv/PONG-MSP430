#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "i2c_oled.h"

volatile uint32_t i;
uint8_t SlaveAddress = 0x3C;
uint8_t TXByteCtr;
uint8_t SlaveFlag = 0;
unsigned char data [2];
unsigned char *TI_transmit_field;
unsigned char *dataBuffer;

//******************************************************************************************************************************************
void i2c_init () {
    P5OUT &= ~(BIT0|BIT1);
    P5DIR |= BIT0|BIT1;

    //Configuração dos pinos I2C (5.0 SDA e 5.1 SCK)
    P5SEL0 |= BIT0|BIT1;                            // I2C pins
    P5SEL1 &= ~(BIT0|BIT1);

    PM5CTL0 &= ~LOCKLPM5;

    // Configurar UCB1
    UCB1CTLW0 |= UCSWRST;                             // botar no estado de reset
    UCB1CTLW0 |= UCMODE_3 | UCMST;                    // Modo I2C master
    UCB1BRW = 0x8;                                    // baudrate = SMCLK / 8
    UCB1CTLW0 &=~ UCSWRST;                            // limpar reset
    UCB1IE |= UCTXIE0 | UCNACKIE;                     // nterrupcao de transmissao e NAKC

    __enable_interrupt();
}
//******************************************************************************************************************************************
void i2c_transmit (unsigned char *params, unsigned char flag) {

    TI_transmit_field = params;

    __delay_cycles(5000);
    UCB1I2CSA = SlaveAddress;              
    TXByteCtr = flag;                                 // qntd de bytes a serem transmitidos
    while (UCB1CTLW0 & UCTXSTP);                      // Verificar envio do stop
    UCB1CTLW0 |= UCTR | UCTXSTT;                      // enviar star para transmissao

    __delay_cycles(2500);

}
//******************************************************************************************************************************************
// I2C interrupt service routine
#pragma vector = USCI_B1_VECTOR
__interrupt void USCIB0_ISR(void) {
  switch(__even_in_range(UCB1IV,USCI_I2C_UCBIT9IFG)) {
  case USCI_I2C_UCNACKIFG:
      UCB0CTL1 |= UCTXSTT;                        // reenviar start caso nack
      break;                                     
  case USCI_I2C_UCTXIFG0:
      if (TXByteCtr)  {                              // enviar quantidade de bytes especificada
          UCB1TXBUF = *TI_transmit_field;
          TI_transmit_field++;
          TXByteCtr--;                              // Decrement TX byte counter
      } else {
          UCB1CTLW0 |= UCTXSTP;                     // envar stop
          UCB1IFG &= ~UCTXIFG;                      
      }
      break;                                     
  }
}
