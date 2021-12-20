#include "helpers.h"

std::string calcSquareColor(int row, int col) {
  return (row + col) % 2 == 0 ? WHITE : BLACK;
}

int sanitizeBoardLength(int number) {
  return std::max(0, std::min(number, BOARD_LENGTH - 1));
}
