#ifndef GAME_H
#define GAME_H
#include "board.h"
#include "computer.h"
#include <memory>

class Game {
private:
  std::shared_ptr<Board> board;
  Computer computer;
  std::string playerColor;
  std::string computerColor;

public:
  Game();

  GameInfo makeComputerMove();

  void newGame(std::string color, int timePerMove);

  GameInfo makeAMove(int startR, int startC, int endR, int endC);

  void setPromotionType(std::string type);

  std::vector<Square> calcAndGetLegalMoves(int row, int col);

  std::vector<std::vector<Square>> getSquares() const;

  std::string getTurn();
};

#endif // GAME_H