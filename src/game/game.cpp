#include "include/game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "include/gameover_state.hpp"
#include <Windows.h>
#include "SDL2/SDL_syswm.h"

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
        startButton_.x = 300; 
        startButton_.y = 300;
        startButton_.w = 200; 
        startButton_.h = 50;  
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
                    // Add the logic to restart the game here
                    break;
                case 2: // Exit Application
                    MessageBox(nullptr, "The application will now close.", "Exit Application", MB_OK);
                    running = false; // This will exit the main game loop
                    break;
                case 3: // Game Controls
                    MessageBox(nullptr,
                        "Game Controls:\n\n"
                        "- To Start Press 'S'\n"
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
                            Player* player = dynamic_cast<Player*>(character.get()); // Assuming players_ stores pointers
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
                for (const auto& enemy : enemies_) {
                    SDL_Rect enemyRect = enemy->getRect(); 
                    SDL_Rect playerRect = player->getRect(); 

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
               // player->updateTexturePos(renderer_);
                player->animateSprite(renderer_);
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
