#include "game.h"

Game::Game() { board = std::make_shared<Board>(); }

void Game::newGame(std::string playerColor, int timePerMove) {
  this->playerColor = playerColor;
  computerColor = playerColor == WHITE ? BLACK : WHITE;
  board = std::make_shared<Board>();
  computer =
      Computer(board, computerColor, std::chrono::milliseconds(timePerMove));
}

GameInfo Game::makeAMove(int startR, int startC, int endR, int endC) {

  return board->makeAMove(startR, startC, endR, endC);
}

void Game::setPromotionType(std::string type) { board->setPromotionType(type); }

std::vector<Square> Game::calcAndGetLegalMoves(int r, int c) {
  if (board->getTurn() != playerColor) {
    return {};
  }
  return board->calcAndGetLegalMoves(r, c);
}

std::vector<std::vector<Square>> Game::getSquares() const {
  return board->getSquares();
}

std::string Game::getTurn() { return board->getTurn(); }

GameInfo Game::makeComputerMove() {

  if (board->getTurn() != computerColor) {
    return board->getGameInfo();
  }

  auto move = computer.getMove();
  auto moves = board->calcAndGetLegalMoves(move.startRow, move.startCol);
  return board->makeAMove(move.startRow, move.startCol, move.endRow,
                          move.endCol);
}