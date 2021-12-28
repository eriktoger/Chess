#include "board.h"
Board::Board() {
  std::vector<std::vector<Square>> vec(8, std::vector<Square>(8));

  squares = vec;

  // white pawns
  for (int i = 0; i < 8; i++) {
    auto piece = Piece(PAWN, WHITE);
    squares[6][i] = Square(6, i, piece);
  }

  // black pawns
  for (int i = 0; i < 8; i++) {
    auto piece = Piece(PAWN, BLACK);
    squares[1][i] = Square(1, i, piece);
  }

  // white bishops
  auto whiteBishop = Piece(BISHOP, WHITE);
  squares[7][2] = Square(7, 2, whiteBishop);
  squares[7][5] = Square(7, 5, whiteBishop);

  // black bishops
  auto blackBishop = Piece(BISHOP, BLACK);
  squares[0][2] = Square(0, 2, blackBishop);
  squares[0][5] = Square(0, 5, blackBishop);

  // white knights
  auto whiteKnight = Piece(KNIGHT, WHITE);
  squares[7][1] = Square(7, 1, whiteKnight);
  squares[7][6] = Square(7, 6, whiteKnight);

  // black knights
  auto blackKnight = Piece(KNIGHT, BLACK);
  squares[0][1] = Square(0, 1, blackKnight);
  squares[0][6] = Square(0, 6, blackKnight);

  // white rooks
  auto whiteRook = Piece(ROOK, WHITE);
  squares[7][0] = Square(7, 0, whiteRook);
  squares[7][7] = Square(7, 7, whiteRook);

  // black Rook
  auto blackRook = Piece(ROOK, BLACK);
  squares[0][0] = Square(0, 0, blackRook);
  squares[0][7] = Square(0, 7, blackRook);

  // white queen
  auto whiteQueen = Piece(QUEEN, WHITE);
  squares[7][3] = Square(7, 3, whiteQueen);

  // black queen
  auto blackQueen = Piece(QUEEN, BLACK);
  squares[0][3] = Square(0, 3, blackQueen);

  // king placeholders
  squares[7][4] = Square(7, 4);
  squares[0][4] = Square(0, 4);

  // squares with out a piece
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      squares[i][j] = Square(i, j);
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

  const auto &type = currentSquare.getPiece().getType();

  possibleSquares.clear();

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  if (!isPlayersPiece) {
    return possibleSquares;
  }

  if (type == PAWN) {
    findPossiblePawnMoves();
  }

  if (type == BISHOP) {
    findPossibleLinearMoves(bishopMovement);
  }

  if (type == KNIGHT) {
    findPossibleKnightMoves();
  }

  if (type == ROOK) {
    findPossibleLinearMoves(rookMovement);
  }

  if (type == QUEEN) {
    findPossibleLinearMoves(bishopMovement);
    findPossibleLinearMoves(rookMovement);
  }

  return possibleSquares;
}

void Board::findPossiblePawnMoves() {

  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();
  const auto &pieceColor = currentSquare.getPiece().getColor();
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
  if (isInsideBoard(row + rowMultiplier, col - 1)) {
    const auto leftSquare = squares[row + rowMultiplier][col - 1];
    const auto leftPiece = leftSquare.getPiece();
    if (leftPiece.getColor() != pieceColor && leftPiece.getType() != "") {
      possibleSquares.emplace_back(leftSquare);
    }
  }

  // can take right
  if (isInsideBoard(row + rowMultiplier, col + 1)) {
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

bool Board::addPossibleSquare(int currentRow, int currentCol,
                              std::string pieceColor) {

  if (!isInsideBoard(currentRow, currentCol)) {
    return false;
  }

  const auto square = squares[currentRow][currentCol];
  const auto currentPieceColor = square.getPiece().getColor();

  if (currentPieceColor == pieceColor) {
    return false;
  }

  if (currentPieceColor != "" && currentPieceColor != pieceColor) {
    possibleSquares.emplace_back(square);
    return false;
  }

  possibleSquares.emplace_back(square);
  return true;
}

void Board::findPossibleLinearMoves(const std::vector<Movement> &movements) {

  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();
  const auto &pieceColor = currentSquare.getPiece().getColor();

  for (const auto &movement : movements) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;
    auto continueFindingMoves = true;

    while (continueFindingMoves) {
      continueFindingMoves =
          addPossibleSquare(currentRow, currentCol, pieceColor);
      currentRow += movement.rowDiff;
      currentCol += movement.colDiff;
    }
  }
}

void Board::findPossibleKnightMoves() {

  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();
  const auto &pieceColor = currentSquare.getPiece().getColor();

  for (const auto &movement : knightMovement) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;
    auto continueFindingMoves = true;

    addPossibleSquare(currentRow, currentCol, pieceColor);
  }
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