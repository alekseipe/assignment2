#include "include/game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "include/gameover_state.hpp"
#include <Windows.h>
#include "SDL2/SDL_syswm.h"
#include "../collectables/include/collectable.hpp"

class MenuState {
public:
    MenuState(SDL_Renderer* renderer) : renderer_(renderer) {
        // Load the menu background
        SDL_Surface* tempSurface = IMG_Load("./src/textures/menu.png");
        backgroundTexture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Load the start button image
        tempSurface = IMG_Load("./src/textures/button-start.png");
        buttonTexture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Adjust the position and size of the button
        startButton_.x = 0; 
        startButton_.y = 0;
        startButton_.w = 1000; 
        startButton_.h = 1000;
    }

    ~MenuState() {
        SDL_DestroyTexture(backgroundTexture_);
        SDL_DestroyTexture(buttonTexture_); // Make sure to destroy the button texture
    }

    bool Update() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return false;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (IsInside(mouseX, mouseY, startButton_)) {
                    return false; // Exit menu state
                }
            }
        }
        return true;
    }

    void Render() {

        SDL_RenderCopy(renderer_, backgroundTexture_, nullptr, nullptr);
        SDL_SetRenderDrawColor(renderer_, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer_, &startButton_);
        SDL_RenderCopy(renderer_, buttonTexture_, nullptr, &startButton_);
    }

private:
    SDL_Renderer* renderer_;
    SDL_Texture* backgroundTexture_;
    SDL_Texture* buttonTexture_; // Texture for the button image
    SDL_Rect startButton_;

    bool IsInside(int x, int y, SDL_Rect& rect) {
        return (x > rect.x) && (x < rect.x + rect.w) &&
            (y > rect.y) && (y < rect.y + rect.h);
    }
};

void CreateWin32Menu(SDL_Window* sdlWindow) {
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    if (!SDL_GetWindowWMInfo(sdlWindow, &wmInfo)) {
        return; // Handle the error appropriately
    }

    HMENU hMenu = CreateMenu();

    // File Menu
    HMENU hFileMenu = CreatePopupMenu();
    AppendMenu(hFileMenu, MF_STRING, 1, "Restart Game");
    AppendMenu(hFileMenu, MF_STRING, 2, "Exit Application");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");

    // Game Controls
    AppendMenu(hMenu, MF_STRING, 3, "Game Controls");

    // Help Menu
    HMENU hHelpMenu = CreatePopupMenu();
    AppendMenu(hHelpMenu, MF_STRING, 4, "About AlekseiPerov engine");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hHelpMenu, "Help");
    //Warning Menu

    HMENU hWarningMenu = CreatePopupMenu();
    AppendMenu(hWarningMenu, MF_STRING, 5, "Medical warning");
    AppendMenu(hWarningMenu, MF_STRING, 6, "Age restrictions");
    AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hWarningMenu, "Warning");

    // Game Controls
    AppendMenu(hMenu, MF_STRING, 7, "Add +1 hp");
    SetMenu(wmInfo.info.win.window, hMenu);
}

Game::Game(const std::string name, int window_width, int window_height)
    : window_width_(window_width), window_height_(window_height), name_(std::move(name)), window_(nullptr), renderer_(nullptr) {
    if (SDL_Init(SDL_INIT_EVERYTHING)) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        exit(1);
    }
    window_ = SDL_CreateWindow(name_.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width_, window_height_, SDL_WINDOW_RESIZABLE);
    if (!window_) {
        std::cerr << "Failed to create game window, reason: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        SDL_DestroyWindow(window_);
        std::cerr << "Failed to create a renderer, reason: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }
    CreateWin32Menu(window_);
    SDL_Surface* bgSurface = IMG_Load("./src/textures/grass_field.png");
    backgroundTexture = SDL_CreateTextureFromSurface(renderer_, bgSurface);
    SDL_FreeSurface(bgSurface);

}


void renderScore(SDL_Renderer* renderer, int score) {
    TTF_Font* font_ = TTF_OpenFont("./src/textures/PixelRpgFont-Regular.ttf", 28);

    // Convert health to string
    std::string healthText = "Score: " + std::to_string(score);

    // Render the text to a surface
    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, healthText.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create texture from surface pixels
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Render the texture
    SDL_Rect renderQuad = { 200, 0, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}




void renderHealth(SDL_Renderer* renderer, int health) {
    TTF_Font* font_ = TTF_OpenFont("./src/textures/PixelRpgFont-Regular.ttf", 28); 

    // Convert health to string
    std::string healthText = "Health: " + std::to_string(health);

    // Render the text to a surface
    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, healthText.c_str(), textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create texture from surface pixels
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (textTexture == nullptr) {
        std::cerr << "Unable to create texture from rendered text! SDL Error: " << SDL_GetError() << std::endl;
    }

    // Render the texture
    SDL_Rect renderQuad = { 0, 0, textSurface->w, textSurface->h };
    SDL_RenderCopy(renderer, textTexture, nullptr, &renderQuad);

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
}


Game::~Game() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
    SDL_DestroyTexture(backgroundTexture);

}

bool Game::Play() noexcept {
    bool running = true; // Flag to keep the game running
    bool gameOver = false; // Flag for game over state
    bool inMenu = true; // Flag for menu state
    GameOverState gameOverState(renderer_); // Initialize the game over state
    MenuState menuState(renderer_); // Initialize the menu state
    SDL_Event event; // Event variable for handling events
    Collectable healthCollectable(renderer_, 200, 250); // Position at (200, 200)
    int score = 0;

    Uint32 last_spawn_time = 0;
    const Uint32 initial_spawn_delay = 3000;
    Uint32 spawn_delay = initial_spawn_delay;
    while (running) { // Main game loop
        healthCollectable.render(renderer_);

        // Inside your main game loop or message handling loop
        MSG msg;
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);

            // Inside your message handling loop
            if (msg.message == WM_COMMAND) {
                switch (LOWORD(msg.wParam)) {
                case 1: // Restart Game
                    MessageBox(nullptr, "The game will restart now.", "Restart Game", MB_OK);
                    return true; // Signal to restart the game
                    break;
                case 2: // Exit Application
                    MessageBox(nullptr, "The application will now close.", "Exit Application", MB_OK);
                    running = false; // This will exit the main game loop
                    break;
                case 3: // Game Controls
                    MessageBox(nullptr,
                        "Game Controls:\n\n"
                        "- To Start Press 'S'\n"
                        "- To Shoot Press 'Space'\n"
                        "- Move Right: Press 'D' or Right Arrow Key\n"
                        "- Move Left: Press 'A' or Left Arrow Key\n"
                        "- Move Up: Press 'W' or Up Arrow Key\n"
                        "- Move Down: Press 'S' or Down Arrow Key\n\n"
                        "Navigate your character through the game using these controls.",
                        "Game Controls", MB_OK);
                    break;
                case 4: // About The Engine
                    MessageBox(nullptr, "The Engine is a powerful SDL2-based C++ 2D game engine.\n\nDeveloped by Aleksei Perov, 2023.", "About My Engine", MB_OK);
                    break;
                case 5: // Medical warning
                    MessageBox(nullptr, "Warning: This game contains flashing lights that may cause discomfort or seizures for people with photosensitive epilepsy. Player discretion is advised.", "Medical Conditions Warning", MB_OK);
                    break;
                case 6: // Age restrictions
                    MessageBox(nullptr, "Please note that this game may not be suitable for all ages. Player discretion is advised.", "Age Restrictions", MB_OK);
                    break;                
                case 7: // +1 hp
                    MessageBox(nullptr, "Hp will be added", "Add +1 hp", MB_OK);
                    for (auto& character : players_) {
                        Player* player = dynamic_cast<Player*>(character.get());
                        player->setHealth(player->getHealth() + 1); // Increase player's health by 1
                    }
                    break;
                }
            }

        }
        while (SDL_PollEvent(&event)) { // Event polling
            if (event.type == SDL_QUIT) { // Check for quit event
                running = false; // Set running to false to exit the loop
            }

            if (inMenu) {
                // Handle menu-specific events
                if (event.type == SDL_KEYDOWN) {
                    if (event.key.keysym.sym == SDLK_s) { // Assuming 'S' key starts the game
                        inMenu = false; // Exit the menu
                    }
                }
            }
            else if (!gameOver) {
                // Handle game-specific events
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                    case SDLK_d:
                    case SDLK_RIGHT:

                        for (auto& character : players_) {
                            Player* player = dynamic_cast<Player*>(character.get());
                            if (player) {
                                player->moveRight(10);
                            }
                        }
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:

                        for (auto& player : players_)
                            player->moveLeft(10);
                        break;

                    case SDLK_w:
                    case SDLK_UP:

                        for (auto& player : players_)
                            player->moveUp(10);
                        break;

                    case SDLK_s:
                    case SDLK_DOWN:
                        for (auto& player : players_)
                            player->moveDown(10);
                        break;
                    case SDLK_SPACE:
                        for (auto& character : players_) {
                            Player* player = dynamic_cast<Player*>(character.get()); 
                            if (player) {
                                player->shoot(renderer_);
                            }
                        }
                        break;
                    }
                } else if (event.type == SDL_KEYUP) {
                    switch (event.key.keysym.sym) {
                    case SDLK_d:
                    case SDLK_RIGHT:
                    case SDLK_a:
                    case SDLK_LEFT:
                    case SDLK_w:
                    case SDLK_UP:
                    case SDLK_s:
                    case SDLK_DOWN:
                        for (auto& character : players_) {
                            Player* player = dynamic_cast<Player*>(character.get());
                            if (player) {
                                player->stopMoving();
                            }
                        }
                        break;
                    }
                }
            }
            else {
                // Handle game over-specific events
                gameOverState.HandleEvents(event, running, gameOver);
            }
        }


        if (inMenu) {
            // Menu rendering
            SDL_RenderClear(renderer_);
            menuState.Render(); // Render the menu state
            SDL_RenderPresent(renderer_);
        }
        else if (!gameOver) {
            // Normal game logic


            for (const auto& player : players_) {

                for (auto& character : players_) {
                    Player* player = dynamic_cast<Player*>(character.get());
                    SDL_Rect playerRect = player->getRect();
                    if (!healthCollectable.isCollected() && SDL_HasIntersection(&playerRect, healthCollectable.getRect())) {
                        healthCollectable.collect();
                        player->setHealth(player->getHealth() + 1); // Increase player's health by 1
                    }
                }

                for (auto& character : players_) {
                    Player* player = dynamic_cast<Player*>(character.get());
                    if (player) {
                        player->stopMoving();
                    }
                }



                for (const auto& enemy : enemies_) {
                    SDL_Rect enemyRect = enemy->getRect();
                    SDL_Rect playerRect = player->getRect();

                    if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                        if ((player->getHealth() > 0)) {
                            player->setHealth(player->getHealth() - 1); // Increase player's health by 1
                            enemy->death(renderer_);
                        }
                        if ((player->getHealth() == 0)) {
                            gameOver = true;
                            break;
                        }

                    }
                }
                if (gameOver) {
                    break; // Break out of outer loop if game over
                }
            }
            for (auto& enemyPtr : enemies_) {
                Enemy* enemy = dynamic_cast<Enemy*>(enemyPtr.get());
                if (enemy) {
                    enemy->updateMoveTimer(1); // Update enemy movement
                    if (enemy->getX() <= 0) {
                        enemy->death(renderer_); // Trigger death animation
                        // Remove the enemy from the vector
                        continue; // Skip the rest of the loop for this enemy
                    }

                    if (enemy->getX() + enemy->getWidth() < 0) {
                        enemy->setX(window_width_); // Reposition the enemy to the right edge of the screen
                    }

                    enemy->animateSprite(renderer_); // Animate the enemy
                }
            }


            // Render updates
            SDL_RenderClear(renderer_);
            SDL_RenderCopy(renderer_, backgroundTexture, nullptr, nullptr);
            healthCollectable.render(renderer_);
            for (const auto& player : players_) {
                Player* playerPtr = dynamic_cast<Player*>(player.get());
                if (playerPtr) {
                    playerPtr->updateProjectiles();
                    renderHealth(renderer_, player->getHealth());
                    renderScore(renderer_, score);

                    // Check for collisions between projectiles and enemies
                    auto& projectiles = playerPtr->getProjectiles(); // Assume this method exists and returns a reference to the projectiles vector
                    for (auto projIt = projectiles.begin(); projIt != projectiles.end();) {
                        bool projectileHit = false;
                        SDL_Rect projectileRect = (*projIt)->getRect(); // Assume getRect() gives the SDL_Rect of the projectile

                        for (auto enemyIt = enemies_.begin(); enemyIt != enemies_.end();) {
                            Enemy* enemy = dynamic_cast<Enemy*>(enemyIt->get());
                            if (enemy) {
                                SDL_Rect enemyRect = enemy->getRect();  // Store the result in a local variable
                                if (SDL_HasIntersection(&projectileRect, &enemyRect)) {
                                    int enemyHealth = enemy->getHealth();
                                    enemyHealth = enemyHealth - 1;
                                    if (enemyHealth > 0) {
                                        enemy->setHealth(enemyHealth);
                                    }
                                    else {
                                        enemy->death(renderer_);
                                        score++;
                                        enemyIt = enemies_.erase(enemyIt);  // Remove the enemy
                                    }
                                    projectileHit = true;
                                    break;  // Assuming one projectile can only hit one enemy
                                }
                                else {
                                    ++enemyIt;
                                }
                            }
                        }

                        if (projectileHit) {
                            delete* projIt;  // Delete the projectile
                            projIt = projectiles.erase(projIt);  // Remove it from the vector
                        }
                        else {
                            ++projIt;
                        }
                    }


                    // Render player and projectiles
                    playerPtr->renderProjectiles(renderer_);
                    playerPtr->animateSprite(renderer_);
                }
            }

            for (auto it = enemies_.begin(); it != enemies_.end();) {
                Enemy* enemy = dynamic_cast<Enemy*>(it->get());
                if (enemy && enemy->isDead) {
                    it = enemies_.erase(it); // Remove the enemy
                }
                else {
                    if (enemy) {
                        enemy->animateSprite(renderer_);
                    }
                    ++it;
                }
            }

            Uint32 current_time = SDL_GetTicks();
            if (current_time - last_spawn_time > spawn_delay) {
                // Randomly generate Y position for the new enemy
                int randomY = rand() % (window_height_ - 64); // Assuming 64 is the max height of your enemy sprites

                // Randomly choose the type of enemy to spawn
                int enemyType = rand() % 4; // Generates a number between 0 and 3

                switch (enemyType) {
                case 0: {
                    // Spawn AncientSkeleton with 1 HP
                    AddCharacter<Enemy>(48, 48, window_width_ - 50, randomY, "./src/textures/WarriorLeftWalk.png");
                    break;
                }
                case 1: {
                    // Spawn GoblinRider with 2 HP
                    auto skeleton = std::make_unique<Enemy>(window_, renderer_, SDL_Rect{ window_width_ - 50, randomY, 80, 80 }, "./src/textures/AncientSkeletonAttack.png");
                    skeleton->setHealth(2);
                    enemies_.push_back(std::move(skeleton));
                    break;
                }
                case 2: {
                    // Spawn GoblinBeast with 1 HP
                    AddCharacter<Enemy>(48, 48, window_width_ - 50, randomY, "./src/textures/GoblinBeastAttack.png");
                    break;
                }
                case 3: {
                    // Spawn Sorceress with 1 HP
                    AddCharacter<Enemy>(48, 48, window_width_ - 50, randomY, "./src/textures/SorceressLeftAttack.png");
                    break;
                }
                }

                if (spawn_delay > 50) {
                    spawn_delay -= 30; // Decrease delay but not less than 50 ms
                }

                last_spawn_time = current_time;
            }


            SDL_RenderPresent(renderer_);
        }
        else {
            // Game Over rendering
            SDL_RenderClear(renderer_);
            gameOverState.Render();
            SDL_RenderPresent(renderer_);
        }
    }
    return false; // Signal to end the game without restarting

}
