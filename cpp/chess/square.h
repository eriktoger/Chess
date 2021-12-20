#ifndef SQUARE_H
#define SQUARE_H
#include "piece.h"

class Square {
private:
  std::string color;
  int row;
  int col;
  Piece piece;

public:
  Square();
  Square(std::string color, int row, int col, Piece piece);
  Square(std::string color, int row, int col);
  int getRow() const;
  int getCol() const;
  std::string getColor() const;
  Piece getPiece() const;
  void replacePiece(Piece piece);
};

#endif