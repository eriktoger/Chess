#ifndef OPENING_BOOK_H
#define OPENING_BOOK_H
#include "helpers.h"
#include "move.h"
#include "openings.h"
#include "randomizer.h"
#include <memory>
#include <vector>

class Node {
private:
public:
  std::vector<std::shared_ptr<Node>> nexts;
  Node() = default;
  Node(Move move);

  Move move;
  void addLine(std::queue<Move> moves);
};

class OpeningBook {
private:
  std::shared_ptr<Node> currentNode;
  Randomizer randomizer;
  bool isActive;

public:
  OpeningBook();
  bool outOfMoves();
  Move findMove();
  void traverse(Move move);
  bool getIsActive();
  void setIsActive(bool isActive);
};

#endif // OPENING_BOOK_H