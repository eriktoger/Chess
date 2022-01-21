#include "board.h"

Board::Board(std::shared_ptr<Board> boardPtr) {

  squares = boardPtr->getSquares();
  turn = boardPtr->getTurn();
  history = boardPtr->history;
  promotionType = boardPtr->promotionType;
  gameStatus = boardPtr->gameStatus;
  positions = boardPtr->positions;
}

Board::Board() {
  Squares vec(BOARD_LENGTH, std::vector<Square>(BOARD_LENGTH));

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
             std::vector<Square> legalMoves, std::string turn,
             Square currentSquare, std::vector<Move> history)
    : squares(squares), legalMoves(legalMoves), turn(turn),
      currentSquare(currentSquare), history(history) {}

std::vector<Square>
Board::findLegalMoves(const std::vector<Square> &possibleMoves,
                      std::string kingColor, Square takoffSquare) {

  std::vector<Square> moves;
  Square kingSquare = findKing(kingColor);
  for (auto possibleMove : possibleMoves) {
    bool illegalMove =
        isMoveIllegal(kingSquare, takoffSquare, possibleMove, possibleMoves);
    if (!illegalMove) {
      moves.emplace_back(possibleMove);
    }
  }
  return moves;
}

bool Board::isMoveIllegal(Square kingSquare, Square currentSquare,
                          Square possibleMove,
                          const std::vector<Square> &possibleMoves) {
  const auto &type = currentSquare.getPiece().getType();
  const auto &row = currentSquare.getRow();
  const auto &col = currentSquare.getCol();

  auto newBoard = Board(squares, possibleMoves, turn, currentSquare, history);
  newBoard.movePiece(row, col, possibleMove.getRow(), possibleMove.getCol());

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
        std::vector<Square> tempPossibleMoves =
            newBoard.findPossibleMoves(newSquare, true);

        auto kingIsAttacked =
            find_if(begin(tempPossibleMoves), end(tempPossibleMoves),
                    hitsKingSquare) != end(tempPossibleMoves);

        if (kingIsAttacked) {
          return true;
        }
      }
    }
  }
  return false;
}

Square Board::findKing(std::string color) {
  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      if (squares[i][j].getPiece().getType() == KING &&
          squares[i][j].getPiece().getColor() == color) {
        return squares[i][j];
      }
    }
  }
  return Square();
}

std::vector<Square> Board::findPossibleMoves(const Square &takeOffSquare,
                                             bool controllMode) {

  std::vector<Square> possibleMoves;

  const auto &type = takeOffSquare.getPiece().getType();
  if (type == PAWN) {
    findPossiblePawnMoves(possibleMoves, takeOffSquare, controllMode);
  }

  if (type == BISHOP) {
    findPossibleLinearMoves(bishopMovement, possibleMoves, takeOffSquare);
  }

  if (type == KNIGHT) {
    findPossibleKnightMoves(possibleMoves, takeOffSquare);
  }

  if (type == ROOK) {
    findPossibleLinearMoves(rookMovement, possibleMoves, takeOffSquare);
  }

  if (type == QUEEN) {
    findPossibleLinearMoves(queenMovement, possibleMoves, takeOffSquare);
  }

  if (type == KING) {
    findPossibleKingMoves(possibleMoves, takeOffSquare, controllMode);
  }

  return possibleMoves;
}

void Board::findPossiblePawnMoves(std::vector<Square> &possibleMoves,
                                  const Square &takeOffSquare,
                                  bool controllMode) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  const int rowMultiplier = pieceColor == WHITE ? -1 : 1;

  addPawnStep(possibleMoves, takeOffSquare, controllMode);

  addPawnCapture(row + rowMultiplier, col - 1, possibleMoves, takeOffSquare,
                 controllMode);
  addPawnCapture(row + rowMultiplier, col + 1, possibleMoves, takeOffSquare,
                 controllMode);

  if (!controllMode) {
    addEnPassant(possibleMoves, takeOffSquare);
  }
}

void Board::addEnPassant(std::vector<Square> &possibleMoves,
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
      possibleMoves.emplace_back(squares[row + rowMultiplier][lastMove.endCol]);
    }
  }
}

void Board::addPawnStep(std::vector<Square> &possibleMoves,
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
    possibleMoves.emplace_back(oneSquareInfront);
  }

  // can move two steps
  auto isAtStart = takeOffSquare.getRow() == startingRow;

  if (isAtStart) {
    const auto twoSquaresInfront = squares[twoRow][col];
    auto canMoveTwoSteps =
        canMoveOneStep && twoSquaresInfront.getPiece().getType().empty();

    if (canMoveTwoSteps && !controllMode) {
      possibleMoves.emplace_back(twoSquaresInfront);
    }
  }
}

void Board::addPawnCapture(int row, int col, std::vector<Square> &possibleMoves,
                           const Square &takeOffSquare,
                           const bool &controllMode) {
  const auto &pieceColor = takeOffSquare.getPiece().getColor();
  if (isInsideBoard(row, col)) {
    const auto leftSquare = squares[row][col];
    const auto leftPiece = leftSquare.getPiece();
    if ((leftPiece.getColor() != pieceColor && leftPiece.getType() != "") ||
        controllMode) {
      possibleMoves.emplace_back(leftSquare);
    }
  }
}

bool Board::addPossibleMove(int currentRow, int currentCol,
                            std::vector<Square> &possibleMoves,
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
    possibleMoves.emplace_back(square);
    return false;
  }

  possibleMoves.emplace_back(square);
  return true;
}

void Board::findPossibleLinearMoves(const std::vector<Movement> &movements,
                                    std::vector<Square> &possibleMoves,
                                    const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  for (const auto &movement : movements) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;
    auto continueFindingMoves = true;

    while (continueFindingMoves) {
      continueFindingMoves =
          addPossibleMove(currentRow, currentCol, possibleMoves, takeOffSquare);
      currentRow += movement.rowDiff;
      currentCol += movement.colDiff;
    }
  }
}

void Board::findPossibleKnightMoves(std::vector<Square> &possibleMoves,
                                    const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();
  const auto &col = takeOffSquare.getCol();

  for (const auto &movement : knightMovement) {
    auto currentRow = row + movement.rowDiff;
    auto currentCol = col + movement.colDiff;

    addPossibleMove(currentRow, currentCol, possibleMoves, takeOffSquare);
  }
}

void Board::findPossibleKingMoves(std::vector<Square> &possibleMoves,
                                  const Square &takeOffSquare,
                                  bool controllMode) {

  auto color = takeOffSquare.getPiece().getColor();

  auto controlledSquares = findControlledSquares(color, controllMode);

  addKingMoves(possibleMoves, controlledSquares, takeOffSquare);

  addShortCastle(possibleMoves, controlledSquares, takeOffSquare);

  addLongCastle(possibleMoves, controlledSquares, takeOffSquare);
}

void Board::addKingMoves(std::vector<Square> &possibleMoves,
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
      addPossibleMove(currentRow, currentCol, possibleMoves, takeOffSquare);
    }
  }
}

void Board::addShortCastle(std::vector<Square> &possibleMoves,
                           const std::vector<Square> &controlledSquares,
                           const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();

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
    possibleMoves.emplace_back(row, 6);
  }
}

void Board::addLongCastle(std::vector<Square> &possibleMoves,
                          const std::vector<Square> &controlledSquares,
                          const Square &takeOffSquare) {

  const auto &row = takeOffSquare.getRow();

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
    possibleMoves.emplace_back(row, 2);
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

        auto controlled = findPossibleMoves(square, true);

        controlledSquares.insert(end(controlledSquares), begin(controlled),
                                 end(controlled));
      }
    }
  }
  return controlledSquares;
}

void Board::changeTurn() { turn = turn == WHITE ? BLACK : WHITE; }

void Board::movePiece(int startRow, int startCol, int endRow, int endCol) {

  auto movedPiece = squares[startRow][startCol].getPiece();
  movedPiece.moved();

  const auto &pieceTypeOnLandingSquare =
      squares[endRow][endCol].getPiece().getType();
  const auto isEnPassant =
      moveEnPassant(startCol, endCol, pieceTypeOnLandingSquare);

  if (movedPiece.getType() == PAWN && (endRow == 0 || endRow == 7)) {
    squares[endRow][endCol].replacePiece(
        Piece(promotionType, movedPiece.getColor()));
  } else {
    squares[endRow][endCol].replacePiece(movedPiece);
  }

  squares[startRow][startCol].replacePiece(Piece());

  const auto &pieceTypeMoved = currentSquare.getPiece().getType();

  moveCastledRook(endRow, startCol, endCol, movedPiece);

  auto pieceTypeCaptured = isEnPassant ? PAWN : pieceTypeOnLandingSquare;
  history.emplace_back(turn, startRow, startCol, endRow, endCol, pieceTypeMoved,
                       pieceTypeCaptured);
}

bool Board::verifyMove(int startRow, int startCol, int endRow, int endCol) {
  auto isCurrentSquare =
      currentSquare.getRow() == startRow && currentSquare.getCol() == startCol;

  auto squareIter =
      find_if(begin(legalMoves), end(legalMoves),
              [&endRow, &endCol](const Square &square) {
                return square.getRow() == endRow && square.getCol() == endCol;
              });

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  return !isCurrentSquare || squareIter == end(legalMoves) || !isPlayersPiece;
}

std::string Board::calcGameStatus() {

  auto opponentsColor = turn == WHITE ? BLACK : WHITE;

  auto king = findKing(turn);
  auto kingIsInCheck = false;
  auto playerCanMove = false;
  auto searchForKing = [king](Square s) {
    return s.getRow() == king.getRow() && s.getCol() == king.getCol();
  };

  auto yourMatingMaterial = 0;
  auto opponentsMatingMaterial = 0;

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      const auto &square = squares[i][j];
      const auto &piece = square.getPiece();

      if (!kingIsInCheck) {
        kingIsInCheck = isKingInCheck(square, opponentsColor, searchForKing);
      }
      if (piece.getColor() == turn && !playerCanMove) {
        playerCanMove = canMove(square);
      }
      if (opponentsMatingMaterial < 3) {
        opponentsMatingMaterial += calcMatingMaterial(piece, opponentsColor);
      }
      if (yourMatingMaterial < 3) {
        opponentsMatingMaterial += calcMatingMaterial(piece, turn);
      }
    }
  }

  auto fiftyMoveRule = false;
  if (history.size() > 100) {
    auto pawnMoveOrCapture = [](Move m) {
      return m.pieceTypeMoved == PAWN || m.pieceTypeCaptured != "";
    };
    fiftyMoveRule = find_if(end(history) - 100, end(history),
                            pawnMoveOrCapture) == end(history);
  }

  const auto drawByRepetition = calcThreeFoldRepetition();
  if (drawByRepetition) {
    return DRAW_BY_REPETITION;
  }

  if (kingIsInCheck && !playerCanMove) {
    return turn == WHITE ? BLACK_WON : WHITE_WON;
  }

  auto suffcientMatingMaterial =
      yourMatingMaterial > 2 || opponentsMatingMaterial > 2;
  if (!playerCanMove) {
    return DRAW_BY_STALEMATE;
  }
  if (!suffcientMatingMaterial) {
    return DRAW_BY_INSUFFICENT_MATING_MATERIAL;
  }
  if (fiftyMoveRule) {
    return DRAW_BY_50_MOVE_RULE;
  }

  return "";
}

bool Board::calcThreeFoldRepetition() {
  std::string position = "";
  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      const auto &type = squares[i][j].getPiece().getType();
      if (type != "") {
        position += type.at(0);
        position += std::to_string(i);
        position += std::to_string(j);
      }
    }
  }
  position += turn;
  // We dont check for enpassant or castling rights

  if (positions.count(position)) {
    positions.at(position)++;
    if (positions.at(position) >= 3) {
      return true;
    }
  } else {
    positions.insert({position, 1});
  }
  return false;
}

int Board::calcMatingMaterial(const Piece &piece, const std::string &color) {
  if (piece.getColor() == color) {
    return 0;
  }

  if (piece.getType() == KNIGHT) {
    return 1;
  }
  if (piece.getType() == BISHOP) {
    return 2;
  }
  if (piece.getType() == QUEEN || piece.getType() == ROOK ||
      piece.getType() == PAWN) {
    return 3;
  }
  return 0;
}

bool Board::canMove(const Square &square) {
  auto moves = findPossibleMoves(square, false);
  return findLegalMoves(moves, turn, square).size() > 0;
}

bool Board::isKingInCheck(const Square &square, std::string opponentsColor,
                          std::function<bool(Square s)> searchForKing) {
  const auto &piece = square.getPiece();
  if (piece.getColor() == opponentsColor && piece.getType() != "") {
    auto controlledSquares = findPossibleMoves(square, true);
    return find_if(begin(controlledSquares), end(controlledSquares),
                   searchForKing) != end(controlledSquares);
  }
  return false;
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

GameInfo Board::makeAMove(int startR, int startC, int endR, int endC) {

  auto startRow = sanitizeBoardLength(startR);
  auto startCol = sanitizeBoardLength(startC);
  auto endRow = sanitizeBoardLength(endR);
  auto endCol = sanitizeBoardLength(endC);

  auto moveIsInvalid = verifyMove(startRow, startCol, endRow, endCol);
  auto lastMove = findLastMove();
  if (moveIsInvalid) {
    return GameInfo(gameStatus, squares, lastMove);
  }

  movePiece(startRow, startCol, endRow, endCol);
  changeTurn();
  gameStatus = calcGameStatus();
  lastMove = findLastMove();
  return GameInfo(gameStatus, squares, lastMove);
}

void Board::setPromotionType(std::string type) { promotionType = type; }

std::vector<Square> Board::calcAndGetLegalMoves(int r, int c) {

  auto row = sanitizeBoardLength(r);
  auto col = sanitizeBoardLength(c);

  currentSquare = squares[row][col];

  legalMoves.clear();

  auto isPlayersPiece = turn == currentSquare.getPiece().getColor();

  if (!isPlayersPiece || gameStatus != "") {
    return legalMoves;
  }

  auto possibleMoves = findPossibleMoves(currentSquare, false);
  legalMoves = findLegalMoves(possibleMoves, turn, currentSquare);
  return legalMoves;
}

Squares Board::getSquares() const { return squares; }

Square Board::getSquare(int row, int col) const { return squares[row][col]; }

GameInfo Board::getGameInfo() {

  auto lastMove = findLastMove();
  return GameInfo(gameStatus, squares, lastMove);
};

Move Board::findLastMove() {

  if (history.size() > 0) {
    return history.back();
  } else {
    return Move(-1, -1, -1, -1);
  }
}