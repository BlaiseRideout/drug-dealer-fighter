#include <stdexcept>
#include <iostream>
#include <glm/glm.hpp>

#include "game.hpp"

int main(int argc, char **argv) {
  try {
    Game game;
    game.init();
    game.run();
    game.cleanup();
    return 0;
  }
  catch(const std::exception &exc) {
    std::cerr << exc.what() << std::endl;
  }
}