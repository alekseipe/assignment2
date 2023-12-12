#include "src/game/include/game.hpp"

int main() {
  Game game("Doka 2", 1000, 1000);
  game.AddCharacter<Player>(48, 48, 0, 50, "./src/textures/WarriorRightIdle.png");
 //  game.AddCharacter<Enemy>(64, 64, 0, 100, "./src/textures/SkeletonKingLeftWalk.png");
  game.AddCharacter<Enemy>(48, 48, 800, 0, "./src/textures/WarriorRightIdle.png");
  game.Play();
  return 0;
}