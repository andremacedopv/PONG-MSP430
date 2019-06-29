#ifndef SSD1306_LIB_H_
#define SSD1306_LIB_H_

#include <stdint.h>
#include "pong.h"

void ssd1306Init();
void sendCommand (unsigned char command);
void sendData (unsigned char params [], unsigned char flag);
void setCursor (unsigned char x, unsigned char p);
void drawPixel (unsigned char x, unsigned char y, unsigned char clear);
void fillDisplay(unsigned char param);
void drawImage(unsigned char x, unsigned char y, unsigned char sizex, unsigned char sizey, const unsigned char img[], unsigned char invert);
void draw6x8Str(unsigned char x, unsigned char p, const char str[], unsigned char invert, unsigned char underline);
void draw12x16Str(unsigned char x, unsigned char y, const char str[], unsigned char invert);
void printInit();
void printDot(uint8_t x, uint8_t y);
void convertMatrix(unsigned char d[], unsigned char s[2][48]);
void insertDot(unsigned char d[2][48], uint8_t y);
void printBall(TBall ball);
void printPaddle(TPaddle paddle[], TBall ball);
void printDivision(TBall ball);
void printScore(uint32_t score[2]);
void printGame(GameState game);

#endif /* SSD1306_LIB_H_ */
