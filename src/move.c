#include "move.h"

bool makeMove(Position *position, Move move) {
    uint8_t piece = getPiece(position, move.from);
    if (piece == EMPTY) {
        return false; 
    }

    if(getPieceColor(piece) != position->turn) {
        return false;
    }

    uint8_t targetPiece = getPiece(position, move.to);
    if (targetPiece != EMPTY) {
        if(getPieceColor(targetPiece) == getPieceColor(piece))
            return false;
        removePiece(position, move.to);
    }

    removePiece(position, move.from);

    if (move.promotion != EMPTY) {
        setPiece(position, move.to, move.promotion);
    } else {
        // Otherwise, just move the piece to the destination square
        setPiece(position, move.to, piece);
    }


    position->turn = (position->turn == WHITE) ? BLACK : WHITE; // Switch turns
    return true;
}