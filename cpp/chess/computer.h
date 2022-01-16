#ifndef COMPUTER_H
#define COMPUTER_H
#include "board.h"
#include "piecePositions.h"
#include <algorithm>
#include <chrono>
#include <memory>

struct EvalInfo {
  EvalInfo() : move(Move(0, 0, 0, 0)) {}
  EvalInfo(std::shared_ptr<Board> board, Move move, int evaluationScore)
      : board(board), move(move), evaluationScore(evaluationScore) {}
  std::shared_ptr<Board> board;
  Move move;
  int evaluationScore;
  bool operator<(const EvalInfo &b) const {
    return this->evaluationScore < b.evaluationScore;
  }
};

class Computer {
private:
  std::shared_ptr<Board> board;
  std::string color;
  std::chrono::milliseconds timePerMove;

  Move getRandomMove();

  Move getMaxMinMove();

  std::vector<EvalInfo> calcFirstMaxMinBatch();

  Move maxMinSearch(std::vector<EvalInfo> topScores);

  std::vector<EvalInfo> maxMinIteration(EvalInfo &evalInfo);

  std::map<std::string, std::vector<Square>>
  findAllMovablePieces(std::shared_ptr<Board> board);

  int calcEvaluation(std::shared_ptr<Board> board);

  int getCurrentPieceValue(const Piece &piece, int row, int col);

public:
  Computer();
  Computer(std::shared_ptr<Board> board, std::string color,
           std::chrono::milliseconds timePerMove);

  Move getMove();
};

#endif // COMPUTER_H