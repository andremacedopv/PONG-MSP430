#ifndef PONG_H_
#define PONG_H_

#include <stdint.h>

#define HEIGHT 24 // Altura da aea de jogo
#define WIDTH 64 // Largur da area de jogo
#define PLAYER1 0 // Player 1 macro
#define PLAYER2 1 // player 2 macro
#define PADHEI 3 // Altura da raquete
// Movimentos da bola
#define LFTSTR 0 // Esquerda reto
#define RIGSTR 1 // Direita reto
#define LFTUP 2 // Esquerda cima
#define RIGUP 3 // Direita cima
#define LFTDOWN 4 // Esquerda baixo
#define RIGDOWN 5 // Direita baixo

typedef struct Tball{
	uint8_t x, y;
	uint8_t dir;
}TBall;

typedef uint8_t TPaddle;

typedef struct gameState{
	TPaddle paddle[2];
	uint32_t score[2];
	TBall ball;
	uint8_t playArea[HEIGHT][WIDTH];
}GameState;

#endif
