#include "computer.h"
#include "constants.h"

Computer::Computer() {}

Computer::Computer(std::shared_ptr<Board> board, std::string color)
    : board(board), color(color) {}

Move Computer::getMove() { return getRandomMove(); }

Move Computer::getRandomMove() {
  if (board->getTurn() != color) {
    return Move(0, 0, 0, 0);
  }

  std::map<std::string, std::vector<Square>> allMoves;
  board->setPromotionType(QUEEN);

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      const auto &piece = board->getSquare(i, j).getPiece();
      if (piece.getColor() == color) {
        auto moves = board->calcAndGetLegalMoves(i, j);
        if (moves.size() > 0) {
          std::string key = "";
          key += std::to_string(i);
          key += std::to_string(j);
          allMoves.insert({key, moves});
        }
      }
    }
  }

  srand(time(NULL));
  auto pieceIndex = rand() % allMoves.size();

  for (auto const &pair : allMoves) {
    if (pieceIndex == 0) {
      auto squareIndex = rand() % pair.second.size();
      for (auto const &move : pair.second) {
        if (squareIndex == 0) {
          int row = pair.first[0] - '0';
          int col = pair.first[1] - '0';
          return Move(row, col, move.getRow(), move.getCol());
        }
        squareIndex--;
      }
    }
    pieceIndex--;
  }
  return Move(0, 0, 0, 0);
}