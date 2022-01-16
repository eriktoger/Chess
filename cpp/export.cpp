#include "chess/game.h"
#include <emscripten/bind.h>

auto game = std::make_shared<Game>();

std::shared_ptr<Game> getGame() { return game; }

using namespace emscripten;

EMSCRIPTEN_BINDINGS(my_module) {

  emscripten::class_<Square>("Square")
      .property("row", &Square::getRow)
      .property("col", &Square::getCol)
      .property("color", &Square::getColor)
      .property("piece", &Square::getPiece);

  emscripten::class_<Piece>("Piece")
      .property("value", &Piece::getValue)
      .property("type", &Piece::getType)
      .property("color", &Piece::getColor);

  emscripten::class_<GameInfo>("GameInfo")
      .property("status", &GameInfo::getStatus)
      .property("squares", &GameInfo::getSquares);

  emscripten::class_<Board>("BoardPtr")
      .constructor<>()
      .smart_ptr<std::shared_ptr<Board>>("BoardPtr")
      .function("getTurn", &Board::getTurn);

  emscripten::class_<Game>("GamePtr")
      .constructor<>()
      .smart_ptr<std::shared_ptr<Game>>("GamePtr")
      .function("getTurn", &Game::getTurn)
      .function("makeAMove", &Game::makeAMove)
      .function("setPromotionType", &Game::setPromotionType)
      .function("calcAndGetLegalMoves", &Game::calcAndGetLegalMoves)
      .function("getSquares", &Game::getSquares)
      .function("newGame", &Game::newGame)
      .function("makeComputerMove", &Game::makeComputerMove);

  emscripten::register_vector<Piece>("pieceVector");
  emscripten::register_vector<Square>("squareVector");
  emscripten::register_vector<std::vector<Square>>("2dVector");

  emscripten::function("getGame", &getGame);
}
