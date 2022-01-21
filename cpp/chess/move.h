#ifndef MOVE_H
#define MOVE_H
#include <string>

class Move {
public:
  Move() = default;
  Move(std::string player, int startRow, int startCol, int endRow, int endCol,
       std::string pieceTypeMoved, std::string pieceTypeCaptured);
  Move(int startRow, int startCol, int endRow, int endCol);
  std::string player;
  int startRow;
  int startCol;
  int endRow;
  int endCol;
  std::string pieceTypeMoved;
  std::string pieceTypeCaptured;
};

#endif // MOVE_H