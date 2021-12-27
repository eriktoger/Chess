#include "square.h"

Square::Square() {
  color = "";
  row = -1;
  col = -1;
}

Square::Square(int row, int col, Piece piece)
    : row{row}, col{col}, piece{piece} {

  color = calcSquareColor(row, col);
}

Square::Square(int row, int col) : row{row}, col{col} {

  color = calcSquareColor(row, col);
}

int Square::getRow() const { return row; }

int Square::getCol() const { return col; }

void Square::replacePiece(Piece newPiece) { piece = newPiece; }

std::string Square::getColor() const { return color; }

Piece Square::getPiece() const { return piece; }