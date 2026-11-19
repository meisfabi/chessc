#ifndef MOVE_H
#define MOVE_H

#include <inttypes.h>
#include "position.h"
#include <stdbool.h>

typedef struct{
    uint8_t from;
    uint8_t to;
    uint8_t promotion;
    uint8_t flags;
} Move;

bool makeMove(Position *position, Move move);

#endif // MOVE_H