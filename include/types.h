#ifndef TYPES_H
#define TYPES_H

#define EMPTY 0
#define NO_SQUARE 64
#define INVALID_PIECE 255

typedef enum{
    PAWN, // 0
    ROOK, // 1
    KNIGHT, // 2    
    BISHOP, // 3
    QUEEN, // 4
    KING, // 5
    PIECE_TYPE_COUNT, // 6
    NO_PIECE_TYPE = -1 
} PieceType;

typedef enum {
    WHITE, // 0
    BLACK, // 1
    NO_COLOR = -1
} Color;

#endif // TYPES_H