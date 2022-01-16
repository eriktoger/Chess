#include "computer.h"
#include "constants.h"

Computer::Computer() {}

Computer::Computer(std::shared_ptr<Board> board, std::string color,
                   std::chrono::milliseconds timePerMove)
    : board(board), color(color), timePerMove(timePerMove) {}

Move Computer::getMove() {

  if (board->getTurn() != color) {
    return Move(0, 0, 0, 0);
  }

  board->setPromotionType(QUEEN);

  if (timePerMove == std::chrono::milliseconds(0)) {
    return getRandomMove();
  }

  return getMaxMinMove();
}

Move Computer::getRandomMove() {

  srand(time(NULL));
  auto allMovablePieces = findAllMovablePieces(board);
  auto pieceIndex = rand() % allMovablePieces.size();

  for (auto const &pair : allMovablePieces) {
    if (pieceIndex == 0) {
      auto squareIndex = rand() % pair.second.size();
      for (auto const &move : pair.second) {
        if (squareIndex == 0) {
          int row = pair.first[0] - '0';
          int col = pair.first[1] - '0';
          return Move(row, col, move.getRow(), move.getCol());
        }
        squareIndex--;
      }
    }
    pieceIndex--;
  }
  return Move(0, 0, 0, 0);
}

Move Computer::getMaxMinMove() {

  auto topScores = calcFirstMaxMinBatch();

  if (topScores.size() == 0) {
    return Move(0, 0, 0, 0);
  }

  if (topScores.size() == 1) {
    return topScores[0].move;
  }

  return maxMinSearch(topScores);
}

Move Computer::maxMinSearch(std::vector<EvalInfo> topScores) {
  auto startTime = std::chrono::system_clock::now();
  auto currentTime = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      currentTime - startTime);

  std::vector<EvalInfo> nextIteration;
  while (duration < timePerMove) {

    for (auto &evalInfo : topScores) {
      auto iteration = maxMinIteration(evalInfo);
      nextIteration.insert(std::end(nextIteration), std::begin(iteration),
                           std::end(iteration));
    }
    if (nextIteration.size() == 0) {
      break;
    }

    topScores.swap(nextIteration);
    if (topScores.size() >= 27) {
      std::sort(std::begin(topScores), std::end(topScores));
      topScores.erase(topScores.begin(), topScores.end() - 3);
    }

    currentTime = std::chrono::system_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        currentTime - startTime);
  }
  std::sort(std::begin(topScores), std::end(topScores));
  return topScores.back().move;
}

std::vector<EvalInfo> Computer::calcFirstMaxMinBatch() {
  auto allMovablePieces = findAllMovablePieces(board);
  std::vector<EvalInfo> topScores;
  const int SCORE_LIMIT = 3;

  for (auto const &pair : allMovablePieces) {
    for (auto const &move : pair.second) {
      auto boardAfter1Move = std::make_shared<Board>(board);

      int row = pair.first[0] - '0';
      int col = pair.first[1] - '0';
      boardAfter1Move->calcAndGetLegalMoves(row, col);

      auto gameInfo =
          boardAfter1Move->makeAMove(row, col, move.getRow(), move.getCol());

      if (gameInfo.getStatus() == WHITE_WON ||
          gameInfo.getStatus() == BLACK_WON) {
        return {EvalInfo(boardAfter1Move,
                         Move(row, col, move.getRow(), move.getCol()), 1000)};
      }

      int minScore = 999999;
      EvalInfo topEvalInfo;

      auto opponentsAllMovablePieces = findAllMovablePieces(boardAfter1Move);
      for (auto const &opponentsPair : opponentsAllMovablePieces) {
        for (auto const &opponentsMove : opponentsPair.second) {
          auto boardAfter2Moves = std::make_shared<Board>(boardAfter1Move);
          int opponentsRow = opponentsPair.first[0] - '0';
          int opponentsCol = opponentsPair.first[1] - '0';
          boardAfter2Moves->calcAndGetLegalMoves(opponentsRow, opponentsCol);

          auto opponentsGameInfo = boardAfter2Moves->makeAMove(
              opponentsRow, opponentsCol, opponentsMove.getRow(),
              opponentsMove.getCol());

          int currentEvaluation;
          if (opponentsGameInfo.getStatus() == WHITE_WON ||
              opponentsGameInfo.getStatus() == BLACK_WON) {
            currentEvaluation = -1000;
          } else {
            currentEvaluation = calcEvaluation(boardAfter2Moves);
          }
          if (minScore > currentEvaluation) {
            minScore = currentEvaluation;
            topEvalInfo = EvalInfo(boardAfter2Moves,
                                   Move(row, col, move.getRow(), move.getCol()),
                                   currentEvaluation);
          }
        }
      }
      if (topScores.size() <= SCORE_LIMIT) {
        topScores.emplace_back(topEvalInfo);
      } else {
        topScores[0] = topEvalInfo;
      }

      std::sort(std::begin(topScores), std::end(topScores));
    }
  }

  if (topScores.size() > SCORE_LIMIT) {
    topScores.erase(topScores.begin());
  }
  return topScores;
}

std::vector<EvalInfo> Computer::maxMinIteration(EvalInfo &evalInfo) {
  auto allMovablePieces = findAllMovablePieces(evalInfo.board);
  std::vector<EvalInfo> topScores;
  const int SCORE_LIMIT = 3;

  for (auto const &pair : allMovablePieces) {
    for (auto const &move : pair.second) {
      auto boardAfter1Move = std::make_shared<Board>(evalInfo.board);

      int row = pair.first[0] - '0';
      int col = pair.first[1] - '0';
      boardAfter1Move->calcAndGetLegalMoves(row, col);

      auto gameInfo =
          boardAfter1Move->makeAMove(row, col, move.getRow(), move.getCol());

      if (gameInfo.getStatus() == WHITE_WON ||
          gameInfo.getStatus() == BLACK_WON) {
        topScores.emplace_back(boardAfter1Move, evalInfo.move, 1000);
        return topScores;
      }

      int minScore = 999999;
      EvalInfo topEvalInfo;

      auto opponentsAllMovablePieces = findAllMovablePieces(boardAfter1Move);
      for (auto const &opponentsPair : opponentsAllMovablePieces) {
        for (auto const &opponentsMove : opponentsPair.second) {
          auto boardAfter2Moves = std::make_shared<Board>(boardAfter1Move);
          int opponentsRow = opponentsPair.first[0] - '0';
          int opponentsCol = opponentsPair.first[1] - '0';
          boardAfter2Moves->calcAndGetLegalMoves(opponentsRow, opponentsCol);

          auto opponentsGameInfo = boardAfter2Moves->makeAMove(
              opponentsRow, opponentsCol, opponentsMove.getRow(),
              opponentsMove.getCol());

          int currentEvaluation;
          if (opponentsGameInfo.getStatus() == WHITE_WON ||
              opponentsGameInfo.getStatus() == BLACK_WON) {
            currentEvaluation = -1000;
          } else {
            currentEvaluation = calcEvaluation(boardAfter2Moves);
          }
          if (minScore > currentEvaluation) {
            minScore = currentEvaluation;
            topEvalInfo =
                EvalInfo(boardAfter2Moves, evalInfo.move, currentEvaluation);
          }
        }
      }
      if (topScores.size() <= SCORE_LIMIT) {
        topScores.emplace_back(topEvalInfo);
      } else {
        topScores[0] = topEvalInfo;
      }

      std::sort(std::begin(topScores), std::end(topScores));
    }
  }

  if (topScores.size() > SCORE_LIMIT) {
    topScores.erase(topScores.begin());
  }

  return topScores;
}

int Computer::calcEvaluation(std::shared_ptr<Board> currentBoard) {

  int evaluationScore = 0;

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      const auto &piece = currentBoard->getSquare(i, j).getPiece();
      if (piece.getColor() == currentBoard->getTurn()) {
        evaluationScore += getCurrentPieceValue(piece, i, j);
      } else if (piece.getColor() != "") {
        evaluationScore -= getCurrentPieceValue(piece, i, j);
      }
    }
  }
  return evaluationScore;
}

int Computer::getCurrentPieceValue(const Piece &piece, int row, int col) {

  auto currentPieceValue = piece.getValue();

  const auto &pieceColor = piece.getColor();
  const auto &pieceType = piece.getType();
  if (pieceColor == WHITE) {
    if (pieceType == PAWN) {
      currentPieceValue += piecePosition::WHITE_PAWN[row][col];
    } else if (pieceType == KNIGHT) {
      currentPieceValue += piecePosition::WHITE_KNIGHT[row][col];
    } else if (pieceType == BISHOP) {
      currentPieceValue += piecePosition::WHITE_BISHOP[row][col];
    } else if (pieceType == ROOK) {
      currentPieceValue += piecePosition::WHITE_ROOK[row][col];
    } else if (pieceType == QUEEN) {
      currentPieceValue += piecePosition::WHITE_QUEEN[row][col];
    } else if (pieceType == KING) {
      currentPieceValue += piecePosition::WHITE_KING[row][col];
    }
  } else if (pieceColor == BLACK) {
    if (pieceType == PAWN) {
      currentPieceValue += piecePosition::BLACK_PAWN[row][col];
    } else if (pieceType == KNIGHT) {
      currentPieceValue += piecePosition::BLACK_KNIGHT[row][col];
    } else if (pieceType == BISHOP) {
      currentPieceValue += piecePosition::BLACK_BISHOP[row][col];
    } else if (pieceType == ROOK) {
      currentPieceValue += piecePosition::BLACK_ROOK[row][col];
    } else if (pieceType == QUEEN) {
      currentPieceValue += piecePosition::BLACK_QUEEN[row][col];
    } else if (pieceType == KING) {
      currentPieceValue += piecePosition::BLACK_KING[row][col];
    }
  }
  return currentPieceValue;
}

std::map<std::string, std::vector<Square>>
Computer::findAllMovablePieces(std::shared_ptr<Board> currentBoard) {

  std::map<std::string, std::vector<Square>> allMovablePieces;

  for (int i = 0; i < BOARD_LENGTH; i++) {
    for (int j = 0; j < BOARD_LENGTH; j++) {
      const auto &piece = currentBoard->getSquare(i, j).getPiece();
      if (piece.getColor() == currentBoard->getTurn()) {
        auto moves = currentBoard->calcAndGetLegalMoves(i, j);
        if (moves.size() > 0) {
          std::string key = "";
          key += std::to_string(i);
          key += std::to_string(j);
          allMovablePieces.insert({key, moves});
        }
      }
    }
  }

  return allMovablePieces;
}
