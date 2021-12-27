#include "board.h"
Board::Board() {
  std::vector<std::vector<Square>> vec(8, std::vector<Square>(8));

  squares = vec;

  // white pawns
  for (int i = 0; i < 8; i++) {
    auto piece = Piece(PAWN, WHITE);
    auto color = calcSquareColor(6, i);
    auto square = Square(color, 6, i, piece);

    squares[6][i] = square;
  }

  // black pawns
  for (int i = 0; i < 8; i++) {
    auto piece = Piece(PAWN, BLACK);
    auto color = calcSquareColor(1, i);
    auto square = Square(color, 1, i, piece);

    squares[1][i] = square;
  }

  // squares with out a piece
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      auto color = calcSquareColor(i, j);
      auto square = Square(color, i, j);

      squares[i][j] = square;
    }
  }

  // Lets white start
  turn = WHITE;
}

Square Board::getSquare(int r, int c) {

  auto row = sanitizeBoardLength(r);
  auto col = sanitizeBoardLength(c);
  return squares[row][col];
}

std::vector<std::vector<Square>> Board::getSquares() const { return squares; }

std::vector<Square> Board::getPossibleMoves(int r, int c) {

  auto row = sanitizeBoardLength(r);
  auto col = sanitizeBoardLength(c);

  currentSquare = squares[row][col];

  const auto &currentPiece = currentSquare.getPiece();
  auto type = currentPiece.getType();
  auto pieceColor = currentPiece.getColor();

  possibleSquares.clear();

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  if (!isPlayersPiece) {
    return possibleSquares;
  }

  if (type == PAWN) {
    findPossiblePawnMoves(row, col, pieceColor);
  }

  return possibleSquares;
}

void Board::findPossiblePawnMoves(int row, int col,
                                  const std::string &pieceColor) {

  const int rowMultiplier = pieceColor == WHITE ? -1 : 1;
  const int startingRow = pieceColor == WHITE ? 6 : 1;
  const int oneRow = row + 1 * rowMultiplier;
  const int twoRow = row + 2 * rowMultiplier;

  // can move one step
  const auto oneSquareInfront = squares[oneRow][col];
  auto canMoveOneStep = oneSquareInfront.getPiece().getType().empty();
  if (canMoveOneStep) {
    possibleSquares.emplace_back(oneSquareInfront);
  }

  // can move two steps
  auto isAtStart = currentSquare.getRow() == startingRow;

  if (isAtStart) {
    const auto twoSquaresInfront = squares[twoRow][col];
    auto canMoveTwoSteps =
        canMoveOneStep && twoSquaresInfront.getPiece().getType().empty();

    if (canMoveTwoSteps) {
      possibleSquares.emplace_back(twoSquaresInfront);
    }
  }

  // can take left
  if (col > 0) {
    const auto leftSquare = squares[row + rowMultiplier][col - 1];
    const auto leftPiece = leftSquare.getPiece();
    if (leftPiece.getColor() != pieceColor && leftPiece.getType() != "") {
      possibleSquares.emplace_back(leftSquare);
    }
  }

  // can take right
  if (col < BOARD_LENGTH) {
    const auto rightSquare = squares[row + rowMultiplier][col + 1];
    const auto rightPiece = rightSquare.getPiece();
    if (rightPiece.getColor() != pieceColor && rightPiece.getType() != "") {
      possibleSquares.emplace_back(rightSquare);
    }
  }

  // can take en passant
  if (history.size() > 0) {
    const auto lastMove = history.back();
    if (lastMove.pieceTypeMoved == PAWN && lastMove.player != turn &&
        lastMove.endRow == row && lastMove.startRow == twoRow &&
        (col + 1 == lastMove.endCol || col - 1 == lastMove.endCol)) {
      possibleSquares.emplace_back(
          squares[row + rowMultiplier][lastMove.endCol]);
    }
  }

  // can promote
  // implement when we have all pieces

  // cant do anything since it is pinned or the king is in check
  // implement when we have a king
}

void Board::changeTurn() { turn = turn == WHITE ? BLACK : WHITE; }

void Board::movePiece(int startR, int startC, int endR, int endC) {
  auto startRow = sanitizeBoardLength(startR);
  auto startCol = sanitizeBoardLength(startC);
  auto endRow = sanitizeBoardLength(endR);
  auto endCol = sanitizeBoardLength(endC);

  auto isCurrentSquare =
      currentSquare.getRow() == startRow && currentSquare.getCol() == startCol;

  auto squareIter =
      find_if(begin(possibleSquares), end(possibleSquares),
              [&endRow, &endCol](const Square &square) {
                return square.getRow() == endRow && square.getCol() == endCol;
              });

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  if (!isCurrentSquare || squareIter == end(possibleSquares) ||
      !isPlayersPiece) {
    return;
  }

  auto pieceTypeMoved = currentSquare.getPiece().getType();
  auto pieceTypeOnLandingSquare = squareIter->getPiece().getType();
  const auto isEnPassant = pieceTypeMoved == PAWN && startCol != endCol &&
                           pieceTypeOnLandingSquare == "";

  // actully move piece

  squares[endRow][endCol].replacePiece(squares[startRow][startCol].getPiece());
  squares[startRow][startCol].replacePiece(Piece());

  if (isEnPassant) {
    const auto lastMove = history.back();
    squares[lastMove.endRow][lastMove.endCol].replacePiece(Piece());
  }

  // save to history
  auto pieceTypeCaptured = isEnPassant ? PAWN : pieceTypeOnLandingSquare;
  history.emplace_back(turn, startRow, startCol, endRow, endCol, pieceTypeMoved,
                       pieceTypeCaptured);

  // has game ended?

  // if not it is the other players turn
  changeTurn();
}

std::string Board::getTurn() { return turn; }