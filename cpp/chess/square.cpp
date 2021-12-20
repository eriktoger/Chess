#include "square.h"

Square::Square() {
  color = "";
  row = -1;
  col = -1;
}

Square::Square(std::string color, int row, int col, Piece piece)
    : color{color}, row{row}, col{col}, piece{piece} {}

Square::Square(std::string color, int row, int col)
    : color{color}, row{row}, col{col} {}

int Square::getRow() const { return row; }

int Square::getCol() const { return col; }

void Square::replacePiece(Piece newPiece) { piece = newPiece; }

std::string Square::getColor() const { return color; }

Piece Square::getPiece() const { return piece; }