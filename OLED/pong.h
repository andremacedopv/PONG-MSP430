#ifndef __PONG_H__
#define __PONG_H__
#include <stdint.h>
#define HEIGHT     24 // Altura da área de jogo
#define DOWNLIMIT  0
#define UPLIMIT    HEIGHT - 1
#define WIDTH      64 // Largura da área de jogo
#define LFTLIMIT   0
#define RIGLIMIT   WIDTH - 1
#define PLAYER1    0  // Player 1 macro
#define PLAYER2    1  // Player 2 macro
#define PADHEI     3  // Altura da raquete
// Movimentos da bola
#define LFTSTR     0  // Esquerda reto
#define RIGSTR     1  // Direita reto
#define LFTUP      2  // Esquerda cima
#define RIGUP      3  // Direita cima
#define LFTDOWN    4  // Esquerda baixo
#define RIGDOWN    5  // Direita baixo
// Posições iniciais
#define PADINITPOS 12 // Posição inicial da raquete
#define BALLXINIT  32 // Posição inicial da bola no eixo x
#define BALLYINIT  13 // Posição inicial da bola no eixo y
#define DIVIDERPOS 32 // Posição do divisor de área

typedef struct TBall{
    uint8_t x, y;
    uint8_t dir;
}TBall;

typedef uint8_t TPaddle;

typedef struct TGameState{
    TPaddle paddle[2];
    uint32_t score[2];
    TBall ball;
    uint8_t playArea[HEIGHT][WIDTH];
}TGameState;

extern TGameState gameState;

void gameInit();
void drawBall();
void eraseBall();
void drawPaddle(uint8_t player);
void erasePaddle(uint8_t player);
void drawDivider();
void drawPlayArea();
int nextBallx();
int nextBally();
void reverseBallDir();
void redirectBallDir(uint8_t x, uint8_t y);
void ballRespawn(uint8_t player);
int paddlePosition(uint8_t x, uint8_t y);
void moveBall();
void movePaddle(uint8_t player, uint8_t newPos);
void updateGameState(uint8_t paddlePlayer1, uint8_t paddlePlayer2);

#endif
