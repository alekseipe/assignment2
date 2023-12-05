#include "include/game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "include/gameover_state.hpp"


class MenuState {
public:
    MenuState(SDL_Renderer* renderer) : renderer_(renderer) {
        // Load the menu background
        SDL_Surface* tempSurface = IMG_Load("./src/textures/menu.png");
        backgroundTexture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Load the start button image
        tempSurface = IMG_Load("./src/textures/button-start.png"); // Adjust the filename as needed
        buttonTexture_ = SDL_CreateTextureFromSurface(renderer, tempSurface);
        SDL_FreeSurface(tempSurface);

        // Adjust the position and size of the button
        startButton_.x = 300; // Change as needed
        startButton_.y = 300; // Change as needed
        startButton_.w = 200; // Adjust to match the button image's width
        startButton_.h = 50;  // Adjust to match the button image's height
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
}

Game::~Game() {
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void Game::Play() noexcept {
    bool running = true; // Flag to keep the game running
    bool gameOver = false; // Flag for game over state
    bool inMenu = true; // Flag for menu state
    GameOverState gameOverState(renderer_); // Initialize the game over state
    MenuState menuState(renderer_); // Initialize the menu state

    SDL_Event event; // Event variable for handling events

    while (running) { // Main game loop
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

                        for (auto& player : players_)
                            player->moveRight(10);

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
            // Add your game update logic here (e.g., moving players and enemies)

            // Check for collisions between players and enemies
            for (const auto& player : players_) {
                for (const auto& enemy : enemies_) {
                    SDL_Rect enemyRect = enemy->getRect(); // Assuming getRect() returns SDL_Rect of enemy
                    SDL_Rect playerRect = player->getRect(); // Assuming getRect() returns SDL_Rect of player

                    if (SDL_HasIntersection(&playerRect, &enemyRect)) {
                        gameOver = true; // Set game over flag if collision detected
                        break; // Break out of inner loop
                    }
                }
                if (gameOver) {
                    break; // Break out of outer loop if game over
                }
            }
            for (auto& enemy : enemies_) {
                enemy->moveLeft(1); // Adjust this value for speed

                // Check if the enemy has moved past the left edge of the screen
                if (enemy->getX() + enemy->getWidth() < 0) {
                    // Reposition the enemy to the right edge of the screen
                    enemy->setX(window_width_);
                }
            }

            // Render updates
            SDL_RenderClear(renderer_);
            for (const auto& player : players_) {
                player->updateTexturePos(renderer_);
            }
            for (const auto& enemy : enemies_) {
                enemy->updateTexturePos(renderer_);
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
}
