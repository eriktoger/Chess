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

  // white king
  auto whiteKing = Piece(KING, WHITE);
  squares[7][4] = Square(7, 4, whiteKing);

  // black king
  auto blackKing = Piece(KING, BLACK);
  squares[0][4] = Square(0, 4, blackKing);

  // squares with out a piece
  for (int i = 2; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      squares[i][j] = Square(i, j);
    }
  }

  // Lets white start
  turn = WHITE;
}

Board::Board(std::vector<std::vector<Square>> squares,
             std::vector<Square> possibleSquares, std::string turn,
             Square currentSquare, std::vector<Move> history)
    : squares(squares), possibleSquares(possibleSquares), turn(turn),
      currentSquare(currentSquare), history(history) {}

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

  possibleSquares.clear();

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  if (!isPlayersPiece) {
    return possibleSquares;
  }

  auto possibleMoves = findPossibleMoves(currentSquare, false);
  addLegalMoves(possibleMoves);

  return possibleSquares;
}

void Board::addLegalMoves(const std::vector<Square> &possibleMoves) {

  Square kingSquare = findKing();
  const auto &type = currentSquare.getPiece().getType();
  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();

  for (auto possibleMove : possibleMoves) {
    auto newBoard = Board(squares, possibleMoves, turn, currentSquare, history);
    newBoard.movePiece(row, col, possibleMove.getRow(), possibleMove.getCol());
    bool illegalMove =
        isMoveLegal(kingSquare, currentSquare, possibleMove, possibleMoves);

    if (!illegalMove) {
      possibleSquares.emplace_back(possibleMove);
    }
  }
}

bool Board::isMoveLegal(Square kingSquare, Square currentSquare,
                        Square possibleMove,
                        const std::vector<Square> &possibleMoves) {
  const auto &type = currentSquare.getPiece().getType();
  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();

  auto newBoard = Board(squares, possibleMoves, turn, currentSquare, history);
  newBoard.movePiece(row, col, possibleMove.getRow(), possibleMove.getCol());
  bool illegalMove = false;

  // the problem is if the king tries to move, but we have set that king is
  // still in place.
  if (type == KING) {
    kingSquare = possibleMove;
  }

  auto hitsKingSquare = [kingSquare](Square s) {
    return s.getRow() == kingSquare.getRow() &&
           s.getCol() == kingSquare.getCol();
  };

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      auto newSquare = newBoard.squares[i][j];
      auto newType = newSquare.getPiece().getType();
      auto newColor = newSquare.getPiece().getColor();
      if (newType != "" && newColor != turn) {
        std::vector<Square> tempPossibleSquares =
            newBoard.findPossibleMoves(newSquare, true);

        auto kingIsAttacked =
            find_if(begin(tempPossibleSquares), end(tempPossibleSquares),
                    hitsKingSquare) != end(tempPossibleSquares);

        if (kingIsAttacked) {
          return true;
        }
      }
    }
  }
  return false;
}

Square Board::findKing() {
  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      if (squares[i][j].getPiece().getType() == KING &&
          squares[i][j].getPiece().getColor() == turn) {
        return squares[i][j];
      }
    }
  }
  return Square();
}

std::vector<Square> Board::findPossibleMoves(const Square &takeOffSquare,
                                             bool controllMode) {

  std::vector<Square> tempPossibleSquares;

  const auto &type = takeOffSquare.getPiece().getType();
  if (type == PAWN) {
    findPossiblePawnMoves(tempPossibleSquares, takeOffSquare, controllMode);
  }

  if (type == BISHOP) {
    findPossibleLinearMoves(bishopMovement, tempPossibleSquares, takeOffSquare);
  }

  if (type == KNIGHT) {
    findPossibleKnightMoves(tempPossibleSquares, takeOffSquare);
  }

  if (type == ROOK) {
    findPossibleLinearMoves(rookMovement, tempPossibleSquares, takeOffSquare);
  }

  if (type == QUEEN) {
    findPossibleLinearMoves(queenMovement, tempPossibleSquares, takeOffSquare);
  }

  if (type == KING) {
    findPossibleKingMoves(tempPossibleSquares, takeOffSquare, controllMode);
  }

  return tempPossibleSquares;
}

void Board::findPossiblePawnMoves(std::vector<Square> &tempPossibleSquares,
                                  const Square &takeOffSquare,
                                  bool controllMode) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  const int rowMultiplier = pieceColor == WHITE ? -1 : 1;

  addPawnStep(tempPossibleSquares, takeOffSquare, controllMode);

  addPawnCapture(row + rowMultiplier, col - 1, tempPossibleSquares,
                 takeOffSquare, controllMode);
  addPawnCapture(row + rowMultiplier, col + 1, tempPossibleSquares,
                 takeOffSquare, controllMode);

  if (!controllMode) {
    addEnPassant(tempPossibleSquares, takeOffSquare);
  }

  // can promote
}

void Board::addEnPassant(std::vector<Square> &tempPossibleSquares,
                         const Square &takeOffSquare) {
  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  const int rowMultiplier = pieceColor == WHITE ? -1 : 1;
  const int twoRow = row + 2 * rowMultiplier;

  if (history.size() > 0) {
    const auto lastMove = history.back();
    if (lastMove.pieceTypeMoved == PAWN && lastMove.player != turn &&
        lastMove.endRow == row && lastMove.startRow == twoRow &&
        (col + 1 == lastMove.endCol || col - 1 == lastMove.endCol)) {
      tempPossibleSquares.emplace_back(
          squares[row + rowMultiplier][lastMove.endCol]);
    }
  }
}

void Board::addPawnStep(std::vector<Square> &tempPossibleSquares,
                        Square takeOffSquare, bool controllMode) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  const int rowMultiplier = pieceColor == WHITE ? -1 : 1;
  const int startingRow = pieceColor == WHITE ? 6 : 1;
  const int oneRow = row + 1 * rowMultiplier;
  const int twoRow = row + 2 * rowMultiplier;

  // can move one step
  const auto oneSquareInfront = squares[oneRow][col];
  auto canMoveOneStep = oneSquareInfront.getPiece().getType().empty();
  if (canMoveOneStep && !controllMode) {
    tempPossibleSquares.emplace_back(oneSquareInfront);
  }

  // can move two steps
  auto isAtStart = takeOffSquare.getRow() == startingRow;

  if (isAtStart) {
    const auto twoSquaresInfront = squares[twoRow][col];
    auto canMoveTwoSteps =
        canMoveOneStep && twoSquaresInfront.getPiece().getType().empty();

    if (canMoveTwoSteps && !controllMode) {
      tempPossibleSquares.emplace_back(twoSquaresInfront);
    }
  }
}

void Board::addPawnCapture(int row, int col,
                           std::vector<Square> &tempPossibleSquares,
                           const Square &takeOffSquare,
                           const bool &controllMode) {
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  if (isInsideBoard(row, col)) {
    const auto leftSquare = squares[row][col];
    const auto leftPiece = leftSquare.getPiece();
    if ((leftPiece.getColor() != pieceColor && leftPiece.getType() != "") ||
        controllMode) {
      tempPossibleSquares.emplace_back(leftSquare);
    }
  }
}

bool Board::addPossibleSquare(int currentRow, int currentCol,
                              std::vector<Square> &tempPossibleSquares,
                              const Square &takeOffSquare) {

  if (!isInsideBoard(currentRow, currentCol)) {
    return false;
  }

  const auto square = squares[currentRow][currentCol];
  const auto currentPieceColor = square.getPiece().getColor();
  const auto &pieceColor = takeOffSquare.getPiece().getColor();

  if (currentPieceColor == pieceColor) {
    return false;
  }

  if (currentPieceColor != "" && currentPieceColor != pieceColor) {
    tempPossibleSquares.emplace_back(square);
    return false;
  }

  tempPossibleSquares.emplace_back(square);
  return true;
}

void Board::findPossibleLinearMoves(const std::vector<Movement> &movements,
                                    std::vector<Square> &tempPossibleSquares,
                                    const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  for (const auto &movement : movements) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;
    auto continueFindingMoves = true;

    while (continueFindingMoves) {
      continueFindingMoves = addPossibleSquare(
          currentRow, currentCol, tempPossibleSquares, takeOffSquare);
      currentRow += movement.rowDiff;
      currentCol += movement.colDiff;
    }
  }
}

void Board::findPossibleKnightMoves(std::vector<Square> &tempPossibleSquares,
                                    const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  for (const auto &movement : knightMovement) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;

    addPossibleSquare(currentRow, currentCol, tempPossibleSquares,
                      takeOffSquare);
  }
}

void Board::findPossibleKingMoves(std::vector<Square> &tempPossibleSquares,
                                  const Square &takeOffSquare,
                                  bool controllMode) {

  auto color = takeOffSquare.getPiece().getColor();

  auto controlledSquares = findControlledSquares(color, controllMode);

  addKingMoves(tempPossibleSquares, controlledSquares, takeOffSquare);

  addShortCastle(tempPossibleSquares, controlledSquares, takeOffSquare);

  addLongCastle(tempPossibleSquares, controlledSquares, takeOffSquare);
}

void Board::addKingMoves(std::vector<Square> &tempPossibleSquares,
                         const std::vector<Square> &controlledSquares,
                         const Square &takeOffSquare) {
  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  for (const auto &movement : queenMovement) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;
    auto squareIsControlled =
        std::find_if(begin(controlledSquares), end(controlledSquares),
                     [currentRow, currentCol](Square s) {
                       return s.getRow() == currentRow &&
                              s.getCol() == currentCol;
                     }) != end(controlledSquares);
    if (!squareIsControlled) {
      addPossibleSquare(currentRow, currentCol, tempPossibleSquares,
                        takeOffSquare);
    }
  }
}

void Board::addShortCastle(std::vector<Square> &tempPossibleSquares,
                           const std::vector<Square> &controlledSquares,
                           const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  auto notMoved = !squares[row][4].getPiece().getHasMoved() &&
                  !squares[row][7].getPiece().getHasMoved() &&
                  squares[row][4].getPiece().getType() == KING &&
                  squares[row][7].getPiece().getType() == ROOK;
  auto freeSpace = squares[row][5].getPiece().getType() == "" &&
                   squares[row][6].getPiece().getType() == "";
  auto notControlled =
      std::find_if(begin(controlledSquares), end(controlledSquares),
                   [row](Square s) {
                     return (s.getRow() == row && s.getCol() == 5) ||
                            (s.getRow() == row && s.getCol() == 6) ||
                            (s.getRow() == row && s.getCol() == 4);
                   }) == end(controlledSquares);
  if (notMoved && freeSpace && notControlled) {
    tempPossibleSquares.emplace_back(row, 6);
  }
}

void Board::addLongCastle(std::vector<Square> &tempPossibleSquares,
                          const std::vector<Square> &controlledSquares,
                          const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  auto notMoved = !squares[row][4].getPiece().getHasMoved() &&
                  !squares[row][0].getPiece().getHasMoved() &&
                  squares[row][4].getPiece().getType() == KING &&
                  squares[row][0].getPiece().getType() == ROOK;
  auto freeSpace = squares[row][2].getPiece().getType() == "" &&
                   squares[row][3].getPiece().getType() == "";
  auto notControlled =
      std::find_if(begin(controlledSquares), end(controlledSquares),
                   [row](Square s) {
                     return (s.getRow() == row && s.getCol() == 2) ||
                            (s.getRow() == row && s.getCol() == 3) ||
                            (s.getRow() == row && s.getCol() == 4);
                   }) == end(controlledSquares);
  if (notMoved && freeSpace && notControlled) {
    tempPossibleSquares.emplace_back(row, 2);
  }
}

std::vector<Square> Board::findControlledSquares(std::string color,
                                                 bool controllMode) {
  std::vector<Square> controlledSquares;
  if (controllMode) {
    return controlledSquares;
  }
  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      auto square = squares[i][j];
      auto currentColor = square.getPiece().getColor();
      if (square.getPiece().getType() != "" && color != currentColor) {

        auto controlled =
            findPossibleMoves(Square(i, j, square.getPiece()), true);

        controlledSquares.insert(end(controlledSquares), begin(controlled),
                                 end(controlled));
      }
    }
  }
  return controlledSquares;
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

  // actully move piece

  auto movedPiece = squares[startRow][startCol].getPiece();
  movedPiece.moved();
  squares[endRow][endCol].replacePiece(movedPiece);
  squares[startRow][startCol].replacePiece(Piece());

  const auto isEnPassant =
      moveEnPassant(startCol, endCol, pieceTypeOnLandingSquare);

  moveCastledRook(endRow, startCol, endCol, movedPiece);

  // save to history
  auto pieceTypeCaptured = isEnPassant ? PAWN : pieceTypeOnLandingSquare;
  history.emplace_back(turn, startRow, startCol, endRow, endCol, pieceTypeMoved,
                       pieceTypeCaptured);

  // has game ended?
  // if check and has no moves you lose
  // no check and no moves is a draw

  // if not it is the other players turn
  changeTurn();
}

void Board::moveCastledRook(int endRow, int startCol, int endCol,
                            const Piece &movedPiece) {

  if (movedPiece.getType() == KING && abs(endCol - startCol) == 2) {

    auto rookStartCol = endCol == 6 ? 7 : 0;
    auto rookEndCol = endCol == 6 ? 5 : 3;

    auto movedRook = squares[endRow][rookStartCol].getPiece();
    movedRook.moved();
    squares[endRow][rookEndCol].replacePiece(movedRook);
    squares[endRow][rookStartCol].replacePiece(Piece());
  }
}

bool Board::moveEnPassant(int startCol, int endCol,
                          std::string pieceTypeOnLandingSquare) {
  auto pieceTypeMoved = currentSquare.getPiece().getType();
  const auto isEnPassant = pieceTypeMoved == PAWN && startCol != endCol &&
                           pieceTypeOnLandingSquare == "";
  if (isEnPassant) {
    const auto lastMove = history.back();
    squares[lastMove.endRow][lastMove.endCol].replacePiece(Piece());
  }
  return isEnPassant;
}

std::string Board::getTurn() { return turn; }