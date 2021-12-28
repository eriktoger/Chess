#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <iostream>
#include <vector>

constexpr int BOARD_LENGTH = 8;
constexpr char WHITE[] = "White";
constexpr char BLACK[] = "Black";
constexpr char PAWN[] = "Pawn";
constexpr char BISHOP[] = "Bishop";
constexpr char KNIGHT[] = "Knight";
constexpr char ROOK[] = "Rook";
constexpr char QUEEN[] = "Queen";

struct Movement {
  int rowDiff;
  int colDiff;
};

const std::vector<Movement> bishopMovement = {
    {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

const std::vector<Movement> rookMovement = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

const std::vector<Movement> knightMovement = {
    {2, 1}, {1, 2}, {-2, -1}, {-1, -2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}};

#endif