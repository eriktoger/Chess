#include "../chess/board.h"
#include <gtest/gtest.h>

void moveMaker(const std::string &moves, Board &board) {

  std::string move;
  for (char const &c : moves) {
    if ((c >= 'a' && c <= 'h') || (c >= '1' && c <= '8')) {
      move += c;
    }

    if (move.size() == 4) {
      int startRow = '8' - move[1];
      int startCol = move[0] - 'a';
      int endRow = '8' - move[3];
      int endCol = move[2] - 'a';
      // std::cout << startRow << " " << startCol << " " << endRow << " " <<
      // endCol
      //           << std::endl;
      board.getPossibleMoves(startRow, startCol);
      board.movePiece(startRow, startCol, endRow, endCol);

      move = "";
    }
  }
}

TEST(BoardHelper, Parser) {
  Board board;
  std::string moves = "e2-e4 e7-e5 Kng1-f3";
  moveMaker(moves, board);
  auto square = board.getSquare(5, 5);
  EXPECT_EQ(square.getPiece().getType(), KNIGHT);
}

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

  moveMaker("e2-e4 d7-d5 c2-c4", newBoard);

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
  moveMaker("e2-e4 e7-e6 e4-e5 d7-d5", newBoard);

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

TEST(NewBoardTests, Bishop) {
  Board newBoard;
  moveMaker("e2-e4 e7-e6 Bf1-c4 d7-d6", newBoard);

  // Bishop moves
  auto possibleMoves = newBoard.getPossibleMoves(4, 2);
  newBoard.movePiece(4, 2, 5, 1);
  const auto square = newBoard.getSquare(5, 1);

  EXPECT_EQ(possibleMoves.size(), 8);
  EXPECT_EQ(square.getPiece().getType(), BISHOP);
}

TEST(NewBoardTests, Knight) {
  Board newBoard;
  moveMaker("Kng1-h3 e7-e6 Knh3-f4 Knb8-c6", newBoard);

  // Knight moves
  auto possibleMoves = newBoard.getPossibleMoves(4, 5);
  EXPECT_EQ(possibleMoves.size(), 6);
}

TEST(NewBoardTests, Rooks) {
  Board newBoard;
  moveMaker("a2-a4 a7-a5 Ra1-a3 Ra8-Ra6", newBoard);

  // Rook moves
  auto possibleMoves = newBoard.getPossibleMoves(5, 0);
  EXPECT_EQ(possibleMoves.size(), 9);
}

TEST(NewBoardTests, Queens) {
  Board newBoard;
  moveMaker("d2-d4 d7-d5 Qd1-d3 Qd8-d6", newBoard);

  // Queen moves
  auto possibleMoves = newBoard.getPossibleMoves(5, 3);
  EXPECT_EQ(possibleMoves.size(), 16);
}

TEST(NewBoardTests, ShortCastle) {
  Board newBoard;
  moveMaker("g2-g3 g7-g6 Bf1-g2 Bf8-g7 Kng1-f3 Kng8-f6", newBoard);

  // white king short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 2);
  newBoard.movePiece(7, 4, 7, 6);

  // black king short castle
  auto blackKingMoves = newBoard.getPossibleMoves(0, 4);
  EXPECT_EQ(blackKingMoves.size(), 2);
  newBoard.movePiece(0, 4, 0, 6);

  auto emptyWhiteCorner = newBoard.getSquare(7, 7);
  auto emptyBlackCorner = newBoard.getSquare(0, 7);
  EXPECT_EQ(emptyWhiteCorner.getPiece().getType(), "");
  EXPECT_EQ(emptyBlackCorner.getPiece().getType(), "");
}

TEST(NewBoardTests, IllegalShortCastleBecauseRookMoved) {
  Board newBoard;
  moveMaker(
      "g2-g3 g7-g6 Bf1-g2 Bf8-g7 Kng1-f3 Kng8-f6 Rh1-g1 Rh8-g8 Rg1-h1 Rg8-h8",
      newBoard);

  // white king tries to short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 1);
}

TEST(NewBoardTests, IllegalShortCastleBecauseSquareIsAttacked) {
  Board newBoard;

  moveMaker("e2-e4 e7-e5 Kng1-f3 b7-b6 Bf1-a6 Bc8-a6", newBoard);

  // white tries to short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 0);
}

TEST(NewBoardTests, IllegalShortCastleBecauseKingMoved) {
  Board newBoard;

  moveMaker(
      "g2-g3 g7-g6 Bf1-g2 Bf8-g7 Kng1-f3 Kng8-f6 Ke1-f1 Ke8-f8 Kf1-e1 Kf8-e8",
      newBoard);

  // white tries to short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 1);
}

TEST(NewBoardTests, IllegalShortCastleBecauseKingIsCheck) {
  Board newBoard;

  moveMaker(
      "g2-g3 g7-g6 Bf1-g2 Bf8-g7 Kng1-f3 Kng8-f6 Ke1-f1 Ke8-f8 Kf1-e1 Kf8-e8",
      newBoard);

  // white tries to short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 1);
}

TEST(NewBoardTests, IllegalShortCastleBecauseKingLandsInCheck) {
  Board newBoard;

  moveMaker("g2-g3 c7-c6 Bf1-g2 Qd8-b6 f2-f4 f7-f5 Kng1-f3 Kng8-f6", newBoard);

  // white tries to short castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 1);
}

TEST(NewBoardTests, LongCastle) {
  Board newBoard;
  moveMaker(
      "b2-b3 b7-b6 Bc1-b2 Bc8-b7 Knb1-c3 Knb8-c6 d2-d4 d7-d5 Qd1-d3 Qd8-d6",
      newBoard);

  // white king long castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 3);
  newBoard.movePiece(7, 4, 7, 2);

  // black king long castle
  auto blackKingMoves = newBoard.getPossibleMoves(0, 4);
  EXPECT_EQ(blackKingMoves.size(), 3);
  newBoard.movePiece(0, 4, 0, 2);

  auto emptyWhiteCorner = newBoard.getSquare(7, 0);
  auto emptyBlackCorner = newBoard.getSquare(0, 0);
  EXPECT_EQ(emptyWhiteCorner.getPiece().getType(), "");
  EXPECT_EQ(emptyBlackCorner.getPiece().getType(), "");
}

TEST(NewBoardTests, IllegalLongCastleBecauseRookMoved) {
  Board newBoard;

  moveMaker("b2-b3 b7-b6 Bc1-b2 Bc8-b7 Knb1-c3 Knb8-c6 d2-d4 d7-d5 Qd1-d3 "
            "Qd8-d6 Ra1-b1 Ra8-b8 Rb1-a1 Rb8-a8",
            newBoard);

  // white king long castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 2);
}

TEST(NewBoardTests, IllegalLongCastleBecauseKingMoved) {
  Board newBoard;

  moveMaker("b2-b3 b7-b6 Bc1-b2 Bc8-b7 Knb1-c3 Knb8-c6 d2-d4 d7-d5 Qd1-d3 "
            "Qd8-d6 Ke1-d1 Ke8-d8 Kd1-e1 Kd8-e8",
            newBoard);

  // white long castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 2);
}

TEST(NewBoardTests, IllegalLongCastleBecauseKingIsChecked) {
  Board newBoard;

  moveMaker(
      "b2-b3 e7-e5 Bc1-b2 Bf8-b4 Knb1-c3 Knb8-c6 d2-d4 d7-d5 Qd1-d3 Bb4-c3",
      newBoard);

  // white long castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 1);
}
TEST(NewBoardTests, IllegalLongCastleBecauseBecauseSquareIsAttacked) {
  Board newBoard;

  moveMaker(
      "e2-e4 d7-d5 Qd1-g4 Bc8-g4 b2-b3 b7-b6 Bc1-b2 Qd8-d7 Knb1-c3 Knb8-c6",
      newBoard);

  // white long castle
  auto whiteKingMoves = newBoard.getPossibleMoves(7, 4);
  EXPECT_EQ(whiteKingMoves.size(), 0);
}

TEST(NewBoardTests, PinnedPieceCannotMove) {
  Board newBoard;

  moveMaker("e2-e4 d7-d5 Qd1-h5", newBoard);

  // white long castle
  auto fPawn = newBoard.getPossibleMoves(1, 5);
  EXPECT_EQ(fPawn.size(), 0);
}
