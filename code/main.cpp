#include <cstdlib>

#include "../config.h"
#include "bits/GameHub.hpp"

int main() {
  game::GameHub hub;
  hub.run();
  return EXIT_SUCCESS;
}
