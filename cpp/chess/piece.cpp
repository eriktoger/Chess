#include "piece.h"
Piece::Piece(std::string t, std::string c) {
  type = t;
  color = c;
  if (t == PAWN) {
    value = 10;
  } else if (t == KNIGHT) {
    value = 32;
  } else if (t == BISHOP) {
    value = 34;
  } else if (t == ROOK) {
    value = 50;
  } else if (t == QUEEN) {
    value = 90;
  } else if (t == KING) {
    // Im not sure if this is needed since my board stops the game before the
    // king is captured I need to add score if game is won/lost instead
    value = 1000;
  } else {
    value = 0;
  }
  hasMoved = false;
}

Piece ::Piece() {
  type = "";
  value = 0;
  color = "";
  hasMoved = false;
}

std::string Piece::getType() const { return type; }

int Piece::getValue() const { return value; }

std::string Piece::getColor() const { return color; }

void Piece::moved() { hasMoved = true; }

bool Piece::getHasMoved() const { return hasMoved; }