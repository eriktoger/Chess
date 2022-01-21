#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "computer.h"
#include "openingBook.h"
#include <memory>

class Game {
private:
  std::shared_ptr<Board> board;
  Computer computer;
  std::string playerColor;
  std::string computerColor;
  OpeningBook openingBook;

public:
  Game();

  GameInfo makeComputerMove();

  void newGame(std::string color, int timePerMove, bool useOpeningBook);

  GameInfo makeAMove(int startR, int startC, int endR, int endC);

  void setPromotionType(std::string type);

  std::vector<Square> calcAndGetLegalMoves(int row, int col);

  Squares getSquares() const;

  std::string getTurn();
};

#endif // GAME_H