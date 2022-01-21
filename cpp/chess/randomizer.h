#ifndef RANDOMIZER_H
#define RANDOMIZER_H
#include "constants.h"
#include <random>
#include <string>

class Randomizer {
private:
  std::mt19937 engine = std::mt19937(time(nullptr));

public:
  int generateRandomNumber(int start, int end);
  std::string generatePlayerColor();
};

#endif