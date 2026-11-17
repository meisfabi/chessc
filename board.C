#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <assert.h>
#include <stdbool.h>

# define EMPTY 0
# define NO_SQUARE 64


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

typedef struct{
    uint64_t figureBitBoards[2][PIECE_TYPE_COUNT]; 
    uint8_t square[64]; 
    Color turn;
    uint8_t castlingRights;
    uint8_t enPassantSquare;
    uint16_t halfMoveClock;
    uint16_t fullMoveNumber;
} Position;

void initBitboards(uint64_t allPieces[2][PIECE_TYPE_COUNT]) {
 // Initialize the board with pieces for both players
    // Player 1 pieces
    allPieces[WHITE][PAWN] = 0xFFULL << 8;
    allPieces[WHITE][ROOK] = (1ULL << 0) | (1ULL << 7); 
    allPieces[WHITE][KNIGHT] = (1ULL << 1) | (1ULL << 6); 
    allPieces[WHITE][BISHOP] = (1ULL << 2) | (1ULL << 5); 
    allPieces[WHITE][QUEEN] = (1ULL << 3); 
    allPieces[WHITE][KING] = (1ULL << 4); 

    // Player 2 pieces
    allPieces[BLACK][PAWN] = 0xFFULL << 48; 
    allPieces[BLACK][ROOK] = (1ULL << 56) | (1ULL << 63); 
    allPieces[BLACK][KNIGHT] = (1ULL << 57) | (1ULL << 62); 
    allPieces[BLACK][BISHOP] = (1ULL << 58) | (1ULL << 61);
    allPieces[BLACK][QUEEN] = (1ULL << 59); 
    allPieces[BLACK][KING] = (1ULL << 60);  
}

void initSquareArray(uint64_t allPieces[2][PIECE_TYPE_COUNT], uint8_t squares[64]) {
    for(int square = 0; square < 64; square++) {
        squares[square] = 0; // Initialize all squares to empty
        
        uint64_t mask = 1ULL << square;

        for(int color = 0; color < 2; color++) {
            for(int type = 0; type < PIECE_TYPE_COUNT; type++) {
                if(allPieces[color][type] & mask) {
                    squares[square] = 1 + type + 6 * color; 
                }
            }
        }
    }
}

void buildBoard(Position *position) {
    initBitboards(position->figureBitBoards);
    initSquareArray(position->figureBitBoards, position->square);

    position->turn = WHITE;
    position->castlingRights = 0b1111; 
    position->enPassantSquare = NO_SQUARE;
    position->halfMoveClock = 0;
    position->fullMoveNumber = 1;
}

uint8_t getPiece(const Position *position, uint8_t square) {
    if (square >= 64) {
        return -1; // Invalid square
    }
    return position->square[square];
}


Color getPieceColor(uint8_t piece) {
    if (piece == 0) {
        return NO_COLOR; // No piece
    }
    return (Color)((piece - 1) / PIECE_TYPE_COUNT); // Determine color based on piece value
}

PieceType getPieceType(uint8_t piece) {
    if (piece == 0) {
        return NO_PIECE_TYPE; // No piece
    }
    return (PieceType)((piece - 1) % PIECE_TYPE_COUNT); // Determine type based on piece value
}

bool setPiece(Position *position, uint8_t square, uint8_t piece) {
    if (square >= 64) {
        return false; // Invalid square
    }

    if(piece == 0 || piece > 2 * PIECE_TYPE_COUNT) {
        return false; // Invalid piece
    }

    if(position->square[square] != 0) {
        return false; // Square is already occupied
    }
    position->square[square] = piece;
    position->figureBitBoards[getPieceColor(piece)][getPieceType(piece)] |= (1ULL << square);
    return true; 
}

uint8_t removePiece(Position *position, uint8_t square) {
    if (square >= 64) {
        return EMPTY; // Invalid square
    }

    uint8_t piece = position->square[square];
    if(piece == EMPTY) {
        return EMPTY; // No piece to remove
    }

    if(piece > 2 * PIECE_TYPE_COUNT) {
        return EMPTY; 
    }

    position->figureBitBoards[getPieceColor(piece)][getPieceType(piece)] &= ~(1ULL << square);
    position->square[square] = EMPTY; // Mark the square as empty
    return piece; 
}

static void printBoard(uint64_t allPieces[2][PIECE_TYPE_COUNT]) {
    // Print the bit board representation
    for (int row = 7; row >= 0; row--) {
        for (int col = 0; col < 8; col++) {
            uint64_t bitPosition = 1ULL << (row * 8 + col);
            char pieceChar = '.';

            for (int color = 0; color < 2; ++color) {
                for (int type = 0; type < PIECE_TYPE_COUNT; type++) {
                    if (allPieces[color][type] & bitPosition) {
                        switch (type) {
                            case PAWN: pieceChar = 'P'; break;
                            case ROOK: pieceChar = 'R'; break;
                            case KNIGHT: pieceChar = 'N'; break;
                            case BISHOP: pieceChar = 'B'; break;
                            case QUEEN: pieceChar = 'Q'; break;
                            case KING: pieceChar = 'K'; break;
                        }
                        if (color == BLACK) {
                            pieceChar += ('a' - 'A'); // Convert to lowercase for black pieces
                        }
                    }
                }
            }
            printf("%c ", pieceChar);
        }
        printf("\n");
    }
   
}

int main(int argc, char const *argv[])
{
    Position position;
    buildBoard(&position);
    printBoard(position.figureBitBoards);

    assert(position.square[0] == 2);
    assert(position.square[4] == 6);
    assert(position.square[60] == 12);
    assert(position.square[63] == 8);
    assert(position.square[27] == 0);

    printf("Piece at square 7: %d\n", getPiece(&position, 7));
    return 0;
}