#include "randomizer.h"

int Randomizer::generateRandomNumber(int start, int end) {

  std::uniform_int_distribution<int> dist{start, end};
  return dist(engine);
}

std::string Randomizer::generatePlayerColor() {

  std::uniform_int_distribution<int> dist{0, 1};
  return dist(engine) ? WHITE : BLACK;
}