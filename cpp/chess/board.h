#ifndef BOARD_H
#define BOARD_H
#include "helpers.h"
#include "move.h"
#include "square.h"
#include <string>
#include <vector>

class Board {
private:
  std::string turn;
  std::vector<std::vector<Square>> squares;
  std::vector<Square> possibleSquares;
  Square currentSquare;
  std::vector<Move> history;
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

  bool addPossibleSquare(int currentRow, int currentCol,
                         std::vector<Square> &tempPossibleSquares,
                         const Square &takeOffSquare);

  Square findKing();
  void addLegalMoves(const std::vector<Square> &possibleMoves);

  bool isMoveLegal(Square kingSquare, Square currentSquare, Square possibleMove,
                   const std::vector<Square> &possibleMoves);

  std::vector<Square> findControlledSquares(std::string color,
                                            bool controllMode);

  bool moveEnPassant(int startCol, int endCol,
                     std::string pieceTypeOnLandingSquare);
  void moveCastledRook(int endRow, int startCol, int endCol,
                       const Piece &movedPiece);

public:
  Board();
  Board(std::vector<std::vector<Square>> squares,
        std::vector<Square> possibleSquares, std::string turn,
        Square currentSquare, std::vector<Move> history);
  Square getSquare(int row, int col);
  std::vector<std::vector<Square>> getSquares() const;
  std::vector<Square> getPossibleMoves(int row, int col);
  std::string getTurn();
  void movePiece(int startRow, int startCol, int endRow, int endCol);
};
#endif // BOARD_H