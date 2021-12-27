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

TEST(NewBoardTests, PossiblePawnMovesInvolvesCaptures) {
  Board newBoard;
  // e2-e4
  newBoard.getPossibleMoves(6, 4);
  newBoard.movePiece(6, 4, 4, 4);

  // d7-d5
  newBoard.getPossibleMoves(1, 3);
  newBoard.movePiece(1, 3, 3, 3);

  // c2-c4
  newBoard.getPossibleMoves(6, 2);
  newBoard.movePiece(6, 2, 4, 2);

  // check if d5 can capture e4 and c4
  auto possibleMoves = newBoard.getPossibleMoves(3, 3);
  auto e4Pawn =
      find_if(begin(possibleMoves), end(possibleMoves), [](const Square &s) {
        return s.getRow() == 4 && s.getCol() == 4;
      });
  auto c4Pawn =
      find_if(begin(possibleMoves), end(possibleMoves), [](const Square &s) {
        return s.getRow() == 4 && s.getCol() == 2;
      });

  EXPECT_EQ(possibleMoves.size(), 3);
  EXPECT_TRUE(e4Pawn != end(possibleMoves));
  EXPECT_TRUE(c4Pawn != end(possibleMoves));
}

TEST(NewBoardTests, EnPassant) {
  Board newBoard;
  // e2-e4
  newBoard.getPossibleMoves(6, 4);
  newBoard.movePiece(6, 4, 4, 4);

  // e7-e6
  newBoard.getPossibleMoves(1, 4);
  newBoard.movePiece(1, 4, 2, 4);

  // e4-e5
  newBoard.getPossibleMoves(4, 4);
  newBoard.movePiece(4, 4, 3, 4);

  // d7-d5
  newBoard.getPossibleMoves(1, 3);
  newBoard.movePiece(1, 3, 3, 3);

  // check if e5 can en passant d6
  auto possibleMoves = newBoard.getPossibleMoves(3, 4);
  newBoard.movePiece(3, 4, 2, 3);

  auto canMoveToD6 =
      find_if(begin(possibleMoves), end(possibleMoves), [](const Square &s) {
        return s.getRow() == 2 && s.getCol() == 3;
      }) != end(possibleMoves);

  auto d5Square = newBoard.getSquare(3, 3);
  auto d6Square = newBoard.getSquare(2, 3);

  EXPECT_EQ(possibleMoves.size(), 1);
  EXPECT_TRUE(canMoveToD6);
  EXPECT_EQ(d6Square.getPiece().getType(), PAWN);
  EXPECT_EQ(d5Square.getPiece().getType(), "");
}
