#include "src/game/include/game.hpp"

int main() {
    bool restart = false;

    do {
        Game game("Game by 13837", 1000, 1000);
        game.AddCharacter<Player>(48, 48, 0, 50, "./src/textures/WarriorRightIdle.png");
        game.AddCharacter<Enemy>(80, 80, 800, 0, "./src/textures/AncientSkeletonAttack.png");
        restart = game.Play(); // Play the game and check if restart is needed
    } while (restart);

    return 0;
}
