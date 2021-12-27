#ifndef HELPERS_H
#define HELPERS_H
#include "constants.h"
#include <algorithm>
#include <string>

std::string calcSquareColor(int row, int col);

int sanitizeBoardLength(int number);

bool isInsideBoard(int row, int col);

#endif