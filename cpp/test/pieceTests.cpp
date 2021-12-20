#include "../chess/constants.h"
#include "../chess/piece.h"
#include <gtest/gtest.h>

TEST(PieceTests, FullPiece) {
  auto piece = Piece(PAWN, WHITE);
  EXPECT_EQ(piece.getType(), PAWN);
  EXPECT_EQ(piece.getColor(), WHITE);
  EXPECT_EQ(piece.getValue(), 10);
}

TEST(PieceTests, EmpytPiece) {
  auto piece = Piece();
  EXPECT_EQ(piece.getType(), "");
  EXPECT_EQ(piece.getColor(), "");
  EXPECT_EQ(piece.getValue(), 0);
}