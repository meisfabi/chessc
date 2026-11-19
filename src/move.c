#include "move.h"

bool makeMove(Position *position, Move move) {

    if(move.from >= 64 || move.to >= 64 || move.from == move.to) {
        return false;
    }   

    uint8_t piece = getPiece(position, move.from);
    if (piece == EMPTY) {
        return false; 
    }

    if(getPieceColor(piece) != position->turn) {
        return false;
    }

    uint8_t targetPiece = getPiece(position, move.to);
    if (targetPiece != EMPTY && getPieceColor(targetPiece) == getPieceColor(piece)) {
        return false;
    }

    if(getPieceType(piece) == KING) {
        return false; 
    }


    PieceType pieceType = getPieceType(piece);
    uint8_t newPiece = (move.promotion != EMPTY) ? move.promotion : piece;

    bool isPromotionSquare = 
        (position->turn == WHITE && move.to >= 56) ||
        (position->turn == BLACK && move.to <= 7);

    bool mustPromote = (pieceType == PAWN && isPromotionSquare);

    if(mustPromote != (move.promotion != EMPTY)) {
        return false;
    }
    
    if(mustPromote){
        if(move.promotion > 2 * PIECE_TYPE_COUNT || getPieceType(move.promotion) == PAWN || getPieceType(move.promotion) == KING || getPieceColor(move.promotion) != position->turn) {
            return false; 
        }
    }

    if (targetPiece != EMPTY) {
        removePiece(position, move.to);
    }

    // TODO rochade & en passant are not implemented yet

    removePiece(position, move.from);

    setPiece(position, move.to, newPiece);

    if(pieceType == PAWN || targetPiece != EMPTY) {
        position->halfMoveClock = 0;
    } else{
        position->halfMoveClock++; 
    }

    position->fullMoveNumber += (position->turn == BLACK) ? 1 : 0; // Increment full move number after Black's move

    position->turn = (position->turn == WHITE) ? BLACK : WHITE; // Switch turns
    return true;
}