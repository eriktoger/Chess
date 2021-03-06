#include "../chess/game.h"
#include <gtest/gtest.h>

TEST(GameTests, MovePiecesWithoutOpeningBook) {
  Game game;
  game.newGame(WHITE, 1000, false);
  game.calcAndGetLegalMoves(6, 4);
  game.makeAMove(6, 4, 4, 4);
  const auto &piece = game.getSquares()[4][4].getPiece();
  EXPECT_EQ(piece.getType(), PAWN);
  auto blacks_turn = game.getTurn();
  EXPECT_EQ(blacks_turn, BLACK);
  game.makeComputerMove();
  auto whites_turn = game.getTurn();
  EXPECT_EQ(whites_turn, WHITE);
}

TEST(GameTests, MovePiecesWithOpeningBook) {
  Game game;
  game.newGame(WHITE, 1000, true);
  game.calcAndGetLegalMoves(6, 4);
  game.makeAMove(6, 4, 4, 4);
  const auto &piece = game.getSquares()[4][4].getPiece();
  EXPECT_EQ(piece.getType(), PAWN);
  auto blacks_turn = game.getTurn();
  EXPECT_EQ(blacks_turn, BLACK);
  game.makeComputerMove();
  auto whites_turn = game.getTurn();
  EXPECT_EQ(whites_turn, WHITE);
}
