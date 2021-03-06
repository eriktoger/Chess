#ifndef PIECE_H
#define PIECE_H
#include "constants.h"
#include <string>

class Piece {
private:
  std::string type;
  int value;
  std::string color;
  bool hasMoved;

public:
  Piece(std::string t, std::string c);
  Piece();
  std::string getType() const;
  int getValue() const;
  std::string getColor() const;
  void moved();
  bool getHasMoved() const;
};
#endif // PIECE_H