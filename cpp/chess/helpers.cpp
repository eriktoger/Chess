#include "helpers.h"
#include "move.h"

std::string calcSquareColor(int row, int col) {
  return (row + col) % 2 == 0 ? WHITE : BLACK;
}

int sanitizeBoardLength(int number) {
  return std::max(0, std::min(number, BOARD_LENGTH - 1));
}

bool isInsideBoard(int row, int col) {
  return row > -1 && col > -1 && row < BOARD_LENGTH && col < BOARD_LENGTH;
}

std::queue<Move> stringToMoves(const std::string &moves) {

  std::queue<Move> convertedMoves;
  std::string move;

  for (char const &c : moves) {
    if ((c >= 'a' && c <= 'h') || (c >= '1' && c <= '8')) {
      move += c;
    }

    if (move.size() == 4) {
      int startRow = '8' - move[1];
      int startCol = move[0] - 'a';
      int endRow = '8' - move[3];
      int endCol = move[2] - 'a';

      convertedMoves.emplace(startRow, startCol, endRow, endCol);
      move = "";
    }
  }

  return convertedMoves;
}
