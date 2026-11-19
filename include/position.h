#ifndef POSITION_H
#define POSITION_H

#include <inttypes.h>
#include <types.h>

typedef struct{
    uint64_t figureBitBoards[2][PIECE_TYPE_COUNT]; 
    uint8_t square[64]; 
    Color turn;
    uint8_t castlingRights;
    uint8_t enPassantSquare;
    uint16_t halfMoveClock;
    uint16_t fullMoveNumber;
} Position;

#endif // POSITION_H