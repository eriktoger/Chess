#include "chess/board.h"
#include <emscripten/bind.h>
Board board;

std::vector<std::vector<Square>> getSquares() { return board.getSquares(); }

std::vector<Square> getPossibleMoves(int row, int col) {
  return board.getPossibleMoves(row, col);
}

std::vector<std::vector<Square>> movePiece(int startRow, int startCol,
                                           int endRow, int endCol) {
  board.movePiece(startRow, startCol, endRow, endCol);
  return board.getSquares();
}

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {

  emscripten::class_<Square>("Square")
      .property("row", &Square::getRow)
      .property("col", &Square::getCol)
      .property("color", &Square::getColor)
      .property("piece", &Square::getPiece);

  emscripten::class_<Piece>("Piece")
      .property("value", &Piece::getValue)
      .property("type", &Piece::getType)
      .property("color", &Piece::getColor);

  emscripten::class_<Board>("Board").property("pieces", &Board::getSquares);

  emscripten::register_vector<Piece>("pieceVector");
  emscripten::register_vector<Square>("squareVector");
  emscripten::register_vector<std::vector<Square>>("2dVector");

  emscripten::function("getSquares", &getSquares);
  emscripten::function("getPossibleMoves", &getPossibleMoves);
  emscripten::function("movePiece", &movePiece);
}
