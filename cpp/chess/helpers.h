#ifndef HELPERS_H
#define HELPERS_H
#include "constants.h"
#include "move.h"
#include <algorithm>
#include <queue>
#include <string>

std::string calcSquareColor(int row, int col);

int sanitizeBoardLength(int number);

bool isInsideBoard(int row, int col);

std::queue<Move> stringToMoves(const std::string &moves);

#endif