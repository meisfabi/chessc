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

    updateCastlingRights(position, move, piece, targetPiece);

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

    // TODO en passant are not implemented yet

    removePiece(position, move.from);

    setPiece(position, move.to, newPiece);

    if(pieceType == PAWN || targetPiece != EMPTY) {
        position->halfMoveClock = 0;
    } else{
        position->halfMoveClock++; 
    }

    position->fullMoveNumber += (position->turn == BLACK) ? 1 : 0;

    position->turn = (position->turn == WHITE) ? BLACK : WHITE; 
    return true;
}

static void updateCastlingRights(Position *position, Move move, uint8_t piece, uint8_t targetPiece){
    PieceType pieceType = getPieceType(piece);
    if(pieceType == KING){
            position->castlingRights &= ~(0x03 << (2 * position->turn));
    }

    if(pieceType == ROOK){
        if(position->turn == WHITE){
            if(move.from == 0) {
                position->castlingRights &= ~0x02; 
            } else if(move.from == 7) {
                position->castlingRights &= ~0x01; 
            }
        } else {
            if(move.from == 56) {
                position->castlingRights &= ~0x08; 
            } else if(move.from == 63) {
                position->castlingRights &= ~0x04; 
            }
        }
    }
    

    if(targetPiece != EMPTY){
        PieceType targetPieceType = getPieceType(targetPiece);
        if(targetPieceType == ROOK){
            PieceType targetPieceColor = getPieceColor(targetPiece);
            if(targetPieceColor == WHITE){
                if(move.to == 0) {
                    position->castlingRights &= ~0x02; 
                } else if(move.to == 7) {
                    position->castlingRights &= ~0x01; 
                }
            } else {
                if(move.to == 56) {
                    position->castlingRights &= ~0x08; 
                } else if(move.to == 63) {
                    position->castlingRights &= ~0x04; 
                }
            }
        }
    }
    
}