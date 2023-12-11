#include "src/game/include/game.hpp"

int main() {
  Game game("Doka 2", 1000, 1000);
  game.AddCharacter<Player>(48, 48, 0, 0, "./src/textures/WarriorRightWalk.png");
  // game.AddCharacter<Enemy>(100, 100, 500, 0, "./src/textures/enemy.jpg");
 // game.AddCharacter<Enemy>(100, 100, 800, 0, "./src/textures/enemy.jpg");
  game.Play();
  return 0;
}