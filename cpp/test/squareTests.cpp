#include "../chess/constants.h"
#include "../chess/square.h"
#include <gtest/gtest.h>

TEST(SquareTests, DefaultSquare) {
  auto square = Square();
  auto piece = square.getPiece();

  EXPECT_EQ(square.getColor(), "");
  EXPECT_EQ(square.getRow(), -1);
  EXPECT_EQ(square.getCol(), -1);
  EXPECT_EQ(piece.getColor(), "");
  EXPECT_EQ(piece.getType(), "");
}

TEST(SquareTests, WithPiece) {
  auto piece = Piece(PAWN, WHITE);
  auto square = Square(1, 0, piece);
  auto pieceOnSquare = square.getPiece();

  EXPECT_EQ(square.getColor(), BLACK);
  EXPECT_EQ(square.getRow(), 1);
  EXPECT_EQ(square.getCol(), 0);
  EXPECT_EQ(pieceOnSquare.getType(), PAWN);
  EXPECT_EQ(pieceOnSquare.getColor(), WHITE);
}

TEST(SquareTests, WithoutPiece) {

  auto square = Square(1, 0);
  auto pieceOnSquare = square.getPiece();

  EXPECT_EQ(square.getColor(), BLACK);
  EXPECT_EQ(square.getRow(), 1);
  EXPECT_EQ(square.getCol(), 0);
  EXPECT_EQ(pieceOnSquare.getType(), "");
  EXPECT_EQ(pieceOnSquare.getColor(), "");
}

TEST(SquareTests, ReplacePiece) {

  auto piece = Piece(PAWN, WHITE);
  auto square = Square(1, 0, piece);
  auto newPiece = Piece(PAWN, BLACK);
  square.replacePiece(newPiece);
  auto pieceOnSquare = square.getPiece();

  EXPECT_EQ(pieceOnSquare.getColor(), BLACK);
  EXPECT_EQ(pieceOnSquare.getType(), PAWN);
}
