#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include <stdint.h>
#include "pong.h"

void confiAnalogPins();

uint16_t readAnalog(uint8_t player);

TPaddle* updatePaddles();

#endif