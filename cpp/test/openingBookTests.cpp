#include "../chess/game.h"
#include <gtest/gtest.h>

TEST(OpeningBook, FindWhiteMove) {

  OpeningBook openingBook;
  auto move = openingBook.findMove();
  EXPECT_TRUE(move.startRow >= 6 && move.startRow <= 7);
  EXPECT_TRUE(move.startCol >= 0 && move.startCol <= BOARD_LENGTH);
  EXPECT_TRUE(move.endRow >= 4 && move.endRow <= 7);
  EXPECT_TRUE(move.endCol >= 0 && move.endCol <= BOARD_LENGTH);
}

TEST(OpeningBook, FindBlackMove) {

  OpeningBook openingBook;

  openingBook.traverse(Move(6, 4, 4, 4));
  auto move = openingBook.findMove();
  EXPECT_TRUE(move.startRow >= 0 && move.startRow <= 1);
  EXPECT_TRUE(move.startCol >= 0 && move.startCol <= BOARD_LENGTH);
  EXPECT_TRUE(move.endRow >= 0 && move.endRow <= 3);
  EXPECT_TRUE(move.endCol >= 0 && move.endCol <= BOARD_LENGTH);
}

TEST(OpeningBook, Reset) {

  OpeningBook openingBook;

  openingBook.traverse(Move(6, 4, 4, 4));
  openingBook.reset(true);
  auto move = openingBook.findMove();
  EXPECT_TRUE(move.startRow >= 6 && move.startRow <= 7);
  EXPECT_TRUE(move.startCol >= 0 && move.startCol <= BOARD_LENGTH);
  EXPECT_TRUE(move.endRow >= 4 && move.endRow <= 7);
  EXPECT_TRUE(move.endCol >= 0 && move.endCol <= BOARD_LENGTH);
}
