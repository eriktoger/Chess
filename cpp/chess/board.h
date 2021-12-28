#ifndef BOARD_H
#define BOARD_H
#include "helpers.h"
#include "move.h"
#include "square.h"
#include <string>
#include <vector>

class Board {
private:
  std::string turn;
  std::vector<std::vector<Square>> squares;
  std::vector<Square> possibleSquares;
  Square currentSquare;
  std::vector<Move> history;
  void changeTurn();
  void findPossiblePawnMoves();
  void findPossibleLinearMoves(const std::vector<Movement> &movements);
  void findPossibleKnightMoves();
  bool addPossibleSquare(int currentRow, int currentCol,
                         std::string pieceColor);

public:
  Board();
  Square getSquare(int row, int col);
  std::vector<std::vector<Square>> getSquares() const;
  std::vector<Square> getPossibleMoves(int row, int col);
  std::string getTurn();
  void movePiece(int startRow, int startCol, int endRow, int endCol);
};
#endif // BOARD_H