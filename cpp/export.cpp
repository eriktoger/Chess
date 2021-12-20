#include "chess/chess.h"
#include <emscripten/bind.h>

using namespace emscripten;
EMSCRIPTEN_BINDINGS(my_module) { emscripten::function("add", &add); }
