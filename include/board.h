#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>
#include "position.h"
#include <stdbool.h>

void buildBoard(Position *position);

uint8_t getPiece(const Position *position, uint8_t square);

bool setPiece(Position *position, uint8_t square, uint8_t piece);

uint8_t removePiece(Position *position, uint8_t square);

void printBoard(const Position *position);


#endif // BOARD_H