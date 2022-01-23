#include "openingBook.h"

Node::Node(Move move) : move(move) {}

void Node::addLine(std::queue<Move> moves) {

  if (moves.size() > 0) {

    auto move = moves.front();
    moves.pop();

    auto findMove = [move](std::shared_ptr<Node> node) {
      auto &m = node->move;
      return m.startRow == move.startRow && m.startCol == move.startCol &&
             m.endRow == move.endRow && m.endCol == move.endCol;
    };

    nexts.push_back(std::make_shared<Node>(move));
    auto iterator = std::find_if(begin(nexts), end(nexts), findMove);

    if (iterator == std::end(nexts)) {
      auto newNode = std::make_shared<Node>(move);
      newNode->addLine(moves);
      nexts.emplace_back(newNode);
    } else {
      (*iterator)->addLine(moves);
    }
  }
}

OpeningBook::OpeningBook() {
  rootNode = std::make_shared<Node>();

  for (const auto &opening : openings) {
    rootNode->addLine(stringToMoves(opening));
  }
  currentNode = rootNode;
}

bool OpeningBook::outOfMoves() {
  return currentNode == nullptr || currentNode->nexts.size() == 0;
}

Move OpeningBook::findMove() {

  const auto index =
      randomizer.generateRandomNumber(0, currentNode->nexts.size() - 1);
  return currentNode->nexts[index]->move;
}

void OpeningBook::traverse(Move move) {

  if (currentNode == nullptr) {
    return;
  }

  auto findMove = [move](std::shared_ptr<Node> node) {
    auto &m = node->move;
    return m.startRow == move.startRow && m.startCol == move.startCol &&
           m.endRow == move.endRow && m.endCol == move.endCol;
  };
  auto iterator = std::find_if(begin(currentNode->nexts),
                               end(currentNode->nexts), findMove);

  if (iterator == std::end(currentNode->nexts)) {
    currentNode = nullptr;
  } else {
    int index = std::distance(begin(currentNode->nexts), iterator);
    currentNode = currentNode->nexts[index];
  }
}

bool OpeningBook::getIsActive() { return isActive; }

void OpeningBook::reset(bool isActive) {
  this->isActive = isActive;
  currentNode = rootNode;
}
