#ifndef COMPUTER_H
#define COMPUTER_H
#include "board.h"
#include <memory>

class Computer {
private:
  std::shared_ptr<Board> board;
  std::string color;

public:
  Computer();
  Computer(std::shared_ptr<Board> board, std::string color);

  Move getMove();
  Move getRandomMove();
};

#endif // COMPUTER_H