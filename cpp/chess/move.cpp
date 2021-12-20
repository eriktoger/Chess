#include "move.h"

Move::Move(std::string player, int startRow, int startCol, int endRow,
           int endCol, std::string pieceTypeMoved,
           std::string pieceTypeCaptured)
    : player(player), startRow(startRow), startCol(startCol), endRow(endRow),
      endCol(endCol), pieceTypeMoved(pieceTypeMoved),
      pieceTypeCaptured(pieceTypeCaptured) {}