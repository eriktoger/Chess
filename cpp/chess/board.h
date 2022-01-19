#ifndef BOARD_H
#define BOARD_H
#include "helpers.h"
#include "move.h"
#include "square.h"
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

class GameInfo {
  std::string status;
  std::vector<std::vector<Square>> squares;
  Move lastMove;

public:
  std::string getStatus() const { return status; }
  std::vector<std::vector<Square>> getSquares() const { return squares; }
  Move getLastMove() const { return lastMove; }
  GameInfo(std::string status, std::vector<std::vector<Square>> squares,
           Move lastMove)
      : status(status), squares(squares), lastMove(lastMove){};
  GameInfo(){};
};

class Board {
private:
  std::vector<std::vector<Square>> squares;
  std::vector<Square> legalMoves;
  std::string turn;
  Square currentSquare;
  std::vector<Move> history;
  std::string promotionType;
  std::string gameStatus;
  std::map<std::string, int> positions;

  void changeTurn();

  std::vector<Square> findPossibleMoves(const Square &takeOffSquare,
                                        bool controllMode);

  void findPossiblePawnMoves(std::vector<Square> &tempPossibleSquares,
                             const Square &takeOffSquare, bool controllMode);

  void addEnPassant(std::vector<Square> &tempPossibleSquares,
                    const Square &takeOffSquare);

  void addPawnStep(std::vector<Square> &tempPossibleSquares,
                   Square takeOffSquare, bool controllMode);

  void addPawnCapture(int row, int col,
                      std::vector<Square> &tempPossibleSquares,
                      const Square &takeOffSquare, const bool &controllMode);

  void findPossibleLinearMoves(const std::vector<Movement> &movements,
                               std::vector<Square> &tempPossibleSquares,
                               const Square &takeOffSquare);

  void findPossibleKnightMoves(std::vector<Square> &tempPossibleSquares,
                               const Square &takeOffSquare);

  void findPossibleKingMoves(std::vector<Square> &tempPossibleSquares,
                             const Square &takeOffSquare, bool controllMode);

  void addKingMoves(std::vector<Square> &tempPossibleSquares,
                    const std::vector<Square> &controlledSquares,
                    const Square &takeOffSquare);

  void addShortCastle(std::vector<Square> &tempPossibleSquares,
                      const std::vector<Square> &controlledSquares,
                      const Square &takeOffSquare);

  void addLongCastle(std::vector<Square> &tempPossibleSquares,
                     const std::vector<Square> &controlledSquares,
                     const Square &takeOffSquare);

  bool addPossibleMove(int currentRow, int currentCol,
                       std::vector<Square> &tempPossibleSquares,
                       const Square &takeOffSquare);

  Square findKing(std::string color);

  std::vector<Square> findLegalMoves(const std::vector<Square> &possibleMoves,
                                     std::string kingColor,
                                     Square takoffSquare);

  bool isMoveIllegal(Square kingSquare, Square currentSquare,
                     Square possibleMove,
                     const std::vector<Square> &possibleMoves);

  std::vector<Square> findControlledSquares(std::string color,
                                            bool controllMode);

  bool moveEnPassant(int startCol, int endCol,
                     std::string pieceTypeOnLandingSquare);

  void moveCastledRook(int endRow, int startCol, int endCol,
                       const Piece &movedPiece);

  void movePiece(int startRow, int startCol, int endRow, int endCol);

  std::string calcGameStatus();

  bool verifyMove(int startRow, int startCol, int endRow, int endCol);

  bool canMove(const Square &square);

  bool isKingInCheck(const Square &square, std::string opponentsColor,
                     std::function<bool(Square s)> searchForKing);

  int calcMatingMaterial(const Piece &piece, const std::string &color);

  bool calcThreeFoldRepetition();

  Move findLastMove();

public:
  Board();

  Board(std::shared_ptr<Board> boardPtr);

  Board(std::vector<std::vector<Square>> squares,
        std::vector<Square> possibleSquares, std::string turn,
        Square currentSquare, std::vector<Move> history);

  GameInfo makeAMove(int startR, int startC, int endR, int endC);
  void setPromotionType(std::string type);
  std::vector<Square> calcAndGetLegalMoves(int row, int col);
  std::vector<std::vector<Square>> getSquares() const;
  Square getSquare(int row, int col) const;
  std::string getTurn();
  GameInfo getGameInfo();
};
#endif // BOARD_H