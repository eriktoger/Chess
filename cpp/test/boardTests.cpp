#include "../chess/board.h"
#include <gtest/gtest.h>

class BoardTests : public ::testing::Test {

public:
  Board board;
};

TEST_F(BoardTests, GenerateBoard) {

  EXPECT_EQ(board.getSquares().size(), BOARD_LENGTH);
  EXPECT_EQ(board.getSquares()[0].size(), BOARD_LENGTH);
  EXPECT_EQ(board.getTurn(), WHITE);
}

// test that board is black and white.

TEST_F(BoardTests, WhitePawns) {
  for (int i = 0; i < BOARD_LENGTH; i++) {

    const auto &square = board.getSquare(6, i);
    const auto &piece = square.getPiece();
    EXPECT_EQ(piece.getColor(), WHITE);
    EXPECT_EQ(piece.getValue(), 10);
    EXPECT_EQ(piece.getType(), PAWN);
  }
}

TEST_F(BoardTests, BlackPawns) {
  for (int i = 0; i < BOARD_LENGTH; i++) {

    const auto &square = board.getSquare(1, i);
    const auto &piece = square.getPiece();
    EXPECT_EQ(piece.getColor(), BLACK);
    EXPECT_EQ(piece.getValue(), 10);
    EXPECT_EQ(piece.getType(), PAWN);
  }
}

TEST_F(BoardTests, EmptyFields) {
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {

      const auto &square = board.getSquare(i, j);
      const auto &piece = square.getPiece();
      EXPECT_EQ(piece.getColor(), "");
      EXPECT_EQ(piece.getValue(), 0);
      EXPECT_EQ(piece.getType(), "");
    }
  }
}

TEST_F(BoardTests, PossibleMovesPawn) {

  auto possibleMoves = board.getPossibleMoves(6, 4);
  EXPECT_EQ(possibleMoves.size(), 2);
  EXPECT_EQ(possibleMoves[0].getRow(), 5);
  EXPECT_EQ(possibleMoves[0].getCol(), 4);
  EXPECT_EQ(possibleMoves[1].getRow(), 4);
  EXPECT_EQ(possibleMoves[1].getCol(), 4);
}

TEST_F(BoardTests, movePawn) {
  Board newBoard;
  newBoard.getPossibleMoves(6, 4);
  newBoard.movePiece(6, 4, 4, 4);
  const auto &piece = newBoard.getSquare(4, 4).getPiece();
  EXPECT_EQ(piece.getType(), PAWN);
}

TEST(NewBoardTests, cannotMovePawn3Steps) {
  Board newBoard;
  newBoard.getPossibleMoves(6, 4);
  newBoard.movePiece(6, 4, 3, 4);
  const auto &piece = newBoard.getSquare(3, 4).getPiece();
  EXPECT_EQ(piece.getType(), "");
}

TEST(NewBoardTests, cannotMoveWrongPawn) {
  Board newBoard;
  newBoard.getPossibleMoves(6, 4);
  newBoard.movePiece(6, 3, 4, 4);
  const auto &piece = newBoard.getSquare(4, 4).getPiece();
  EXPECT_EQ(piece.getType(), "");
}

TEST(NewBoardTests, cannotMoveOpponentsPiece) {
  Board newBoard;
  newBoard.getPossibleMoves(1, 4);
  newBoard.movePiece(1, 4, 3, 4);
  const auto &piece = newBoard.getSquare(3, 4).getPiece();
  EXPECT_EQ(piece.getType(), "");
}
