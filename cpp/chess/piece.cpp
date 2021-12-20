#include "piece.h"
Piece::Piece(std::string t, std::string c) {
  type = t;
  color = c;
  if (t == PAWN) {
    value = 10;
  } else {
    value = 0;
  }
}
Piece ::Piece() {
  type = "";
  value = 0;
  color = "";
}

std::string Piece::getType() const { return type; }

int Piece::getValue() const { return value; }

std::string Piece::getColor() const { return color; }