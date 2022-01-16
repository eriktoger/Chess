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
constexpr char KING[] = "King";

constexpr char DRAW_BY_STALEMATE[] = "Draw by stalemate";
constexpr char DRAW_BY_INSUFFICENT_MATING_MATERIAL[] =
    "Draw by insufficient mating material";
constexpr char DRAW_BY_REPETITION[] = "Draw by repetition";
constexpr char DRAW_BY_50_MOVE_RULE[] = "Draw by 50 move rule";
constexpr char WHITE_WON[] = "White won";
constexpr char BLACK_WON[] = "Black won";

struct Movement {
  int rowDiff;
  int colDiff;
};

const std::vector<Movement> bishopMovement = {
    {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

const std::vector<Movement> rookMovement = {{1, 0}, {0, 1}, {-1, 0}, {0, -1}};

const std::vector<Movement> queenMovement = {
    {1, 0}, {0, 1}, {-1, 0}, {0, -1}, {-1, -1}, {1, 1}, {-1, 1}, {1, -1}};

const std::vector<Movement> knightMovement = {
    {2, 1}, {1, 2}, {-2, -1}, {-1, -2}, {-2, 1}, {-1, 2}, {2, -1}, {1, -2}};

#endif
