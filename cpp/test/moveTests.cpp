#include "../chess/constants.h"
#include "../chess/move.h"
#include <gtest/gtest.h>

TEST(MoveTests, ConstructMove) {
  auto move = Move(WHITE, 1, 3, 2, 3, PAWN, "");
  EXPECT_EQ(move.player, WHITE);
  EXPECT_EQ(move.startRow, 1);
  EXPECT_EQ(move.startCol, 3);
  EXPECT_EQ(move.endRow, 2);
  EXPECT_EQ(move.endCol, 3);
  EXPECT_EQ(move.pieceTypeMoved, PAWN);
  EXPECT_EQ(move.pieceTypeCaptured, "");
}