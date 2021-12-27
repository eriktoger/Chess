#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>
#include <vector>

constexpr int BOARD_LENGTH = 8;
constexpr char WHITE[] = "White";
constexpr char BLACK[] = "Black";
constexpr char PAWN[] = "Pawn";
constexpr char BISHOP[] = "Bishop";

struct Movement {

  int rowDiff;
  int colDiff;
};

const std::vector<Movement> bishopMovement = {
    {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

#endif