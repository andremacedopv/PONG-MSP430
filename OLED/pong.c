#include <stdio.h>
#include <stdint.h>
#include "pong.h"

TGameState gameState;

void gameInit(){
    gameState.score[PLAYER1] = 0;
    gameState.score[PLAYER2] = 0;
    gameState.paddle[PLAYER1] = PADINITPOS;
    gameState.paddle[PLAYER2] = PADINITPOS;
    gameState.ball.x = BALLXINIT;
    gameState.ball.y = BALLYINIT;
    gameState.ball.dir = LFTUP;
    drawPlayArea();
}

void updateGameState(uint8_t paddlePlayer1, uint8_t paddlePlayer2){
    movePaddle(PLAYER1, paddlePlayer1);
    movePaddle(PLAYER2, paddlePlayer2);
    moveBall();
}

void drawPlayArea(){
    int i, j;
    for(i=0; i<HEIGHT; i++){
        for(j=0; j<WIDTH; j++){
            gameState.playArea[i][j] = 0;
        }
    }
    drawDivider();
    drawBall();
    drawPaddle(PLAYER1);
    drawPaddle(PLAYER2);
}

void drawPaddle(uint8_t player){
    uint8_t col, pos;
    if(player == PLAYER1){
        col = LFTLIMIT;
        pos = gameState.paddle[PLAYER1];
    }
    else if(player == PLAYER2){
        col = RIGLIMIT;
        pos = gameState.paddle[PLAYER2];
    }
    else{
        return;
    }
    gameState.playArea[pos][col] = 1;
    gameState.playArea[pos + ((PADHEI - 1) / 2)][col] = 1;
    gameState.playArea[pos - ((PADHEI - 1) / 2)][col] = 1;
}

void erasePaddle(uint8_t player){
    uint8_t col, pos;
    if(player == PLAYER1){
        col = LFTLIMIT;
        pos = gameState.paddle[PLAYER1];
    }
    else if(player == PLAYER2){
        col = RIGLIMIT;
        pos = gameState.paddle[PLAYER2];
    }
    else{
        return;
    }
    gameState.playArea[pos][col] = 0;
    gameState.playArea[pos + ((PADHEI - 1) / 2)][col] = 0;
    gameState.playArea[pos - ((PADHEI - 1) / 2)][col] = 0;
}

void drawBall(){
    gameState.playArea[gameState.ball.y][gameState.ball.x] = 1;
}

void eraseBall(){
    uint8_t x, y;
    x = gameState.ball.x;
    y = gameState.ball.y;
    if( (x == DIVIDERPOS) && ((y%2) == 0)){ // Verifica se é um píxel do divisor de área
        return;                             // Caso seja não apaga
    }
    else if(paddlePosition(x, y)){          // Verifica se é um píxel da raquete
        return;                             // Caso seja não apaga
    }
    else{
        gameState.playArea[y][x] = 0;
    }
}

void drawDivider(){
    int i;
    for(i=0; i<HEIGHT; i++){
        if( (i % 2) == 0){
            gameState.playArea[i][32] = 1;
        }
    }
}

void moveBall(){
    int8_t x, y;
    x = nextBallx();
    y = nextBally();
    if (y > UPLIMIT || y < DOWNLIMIT){
        reverseBallDir();
    }
    else if(paddlePosition(x, y)){
        redirectBallDir(x, y);
    }
    else if(x > RIGLIMIT){
        ballRespawn(PLAYER2);
        gameState.score[PLAYER1]++;
    }
    else if(x < LFTLIMIT){
        ballRespawn(PLAYER1);
        gameState.score[PLAYER2]++;
    }
    else{
        eraseBall();          // Apaga a bola na posição anterior
        gameState.ball.x = (uint8_t)x; // Atualiza a posição da bola
        gameState.ball.y = (uint8_t)y; 
        drawBall();           // Desenha a bola na nova posição
    }
}

int nextBallx(){
    int x;
    switch (gameState.ball.dir){
    case LFTSTR:
        x = gameState.ball.x - 1;
        break;
    case LFTDOWN:
        x = gameState.ball.x - 1;
        break;
    case LFTUP:
        x = gameState.ball.x - 1;
        break;
    case RIGSTR:
        x = gameState.ball.x + 1;
        break;
    case RIGUP:
        x = gameState.ball.x + 1;
        break;
    case RIGDOWN:
        x = gameState.ball.x + 1;
        break;
    default:
        x = gameState.ball.x;
        break;
    }
    return x;
}

int nextBally(){
    int y;
    switch (gameState.ball.dir){
    case LFTSTR:
        y = gameState.ball.y;
        break;
    case LFTDOWN:
        y = gameState.ball.y + 1;
        break;
    case LFTUP:
        y = gameState.ball.y - 1;
        break;
    case RIGSTR:
        y = gameState.ball.y;
        break;
    case RIGUP:
        y = gameState.ball.y - 1;
        break;
    case RIGDOWN:
        y = gameState.ball.y + 1;
        break;
    default:
        y = gameState.ball.y;
        break;
    }
    return y;
}

void reverseBallDir(){
    switch (gameState.ball.dir){
    case LFTSTR:
        gameState.ball.dir = RIGSTR;
        break;
    case LFTDOWN:
        gameState.ball.dir = LFTUP;
        break;
    case LFTUP:
        gameState.ball.dir = LFTDOWN;
        break;
    case RIGSTR:
        gameState.ball.dir = LFTSTR;
        break;
    case RIGUP:
        gameState.ball.dir = RIGDOWN;
        break;
    case RIGDOWN:
        gameState.ball.dir = RIGUP;
        break;
    default:
        break;
    }
    return;
}

void redirectBallDir(uint8_t x, uint8_t y){
    uint8_t posPlayer1, posPlayer2;
    posPlayer1 = gameState.paddle[PLAYER1];
    posPlayer2 = gameState.paddle[PLAYER2];

    if ( y == (posPlayer1 + ((PADHEI - 1) / 2)) && (x == LFTLIMIT)){
        gameState.ball.dir = RIGDOWN;
    }
    else if ( y == (posPlayer1 - ((PADHEI - 1) / 2)) && (x == LFTLIMIT)){
        gameState.ball.dir = RIGUP;
    }
    else if ( y == (posPlayer2 + ((PADHEI - 1) / 2)) && (x == RIGLIMIT)){
        gameState.ball.dir = LFTDOWN;
    }
    else if ( y == (posPlayer2 - ((PADHEI - 1) / 2)) && (x == RIGLIMIT)){
        gameState.ball.dir = LFTUP;
    }
    else if((y == posPlayer1) && (x == LFTLIMIT)){
        if(gameState.ball.dir == LFTUP)
            gameState.ball.dir = RIGUP;
        else if(gameState.ball.dir == LFTDOWN){
            gameState.ball.dir = RIGDOWN;
        }
        else{
            gameState.ball.dir = RIGSTR;
        }
    }
    else if((y == posPlayer2) && (x == RIGLIMIT)){
        if(gameState.ball.dir == RIGUP)
            gameState.ball.dir = LFTUP;
        else if(gameState.ball.dir == RIGDOWN){
            gameState.ball.dir = LFTDOWN;
        }
        else{
            gameState.ball.dir = LFTSTR;
        }
    }
    else{
        reverseBallDir();
    }
}

void ballRespawn(uint8_t player){
    eraseBall();
    gameState.ball.x = BALLXINIT;
    gameState.ball.y = BALLYINIT;
    drawBall();
    if (player == PLAYER1){
        gameState.ball.dir = LFTSTR;
    }
    else if (player == PLAYER2){
        gameState.ball.dir = RIGSTR;
    }
    else{
        return;
    }
}

int paddlePosition(uint8_t x, uint8_t y){
    uint8_t pos, y1, y2, y3;
    pos = gameState.paddle[PLAYER1];
    y1 = pos + ((PADHEI - 1) / 2);
    y2 = pos;
    y3 = pos - ((PADHEI - 1) / 2);
    if( ((y == y1) || (y == y2) || (y == y3)) && (x == LFTLIMIT)){
        return 1;
    }
    pos = gameState.paddle[PLAYER2];
    y1 = pos + ((PADHEI - 1) / 2);
    y2 = pos;
    y3 = pos - ((PADHEI - 1) / 2);
    if( ((y == y1) || (y == y2) || (y == y3)) && (x == RIGLIMIT)){
        return 1;
    }
    return 0;
}

void movePaddle(uint8_t player, uint8_t newPos){
    if(((player != PLAYER1) && (player != PLAYER2)) || (newPos < (0 + ((PADHEI - 1) / 2)))  || (newPos > (UPLIMIT - ((PADHEI - 1) / 2)))){
        return;
    }
    erasePaddle(player);               // Apaga a raquete na posição anterior
    gameState.paddle[player] = newPos; // Atualiza a posição da raquete
    drawPaddle(player);                // Desenha a raquete na nova posição
}
