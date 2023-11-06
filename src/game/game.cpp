#include "include/game.hpp"

#include <iostream>

Game::Game(const std::string name, int window_width, int window_height) 
: window_width_(window_width), window_height_(window_height), name_(std::move(name)), window_(nullptr), renderer_(nullptr) {
  // initialize SDL internals
  if (SDL_Init(SDL_INIT_EVERYTHING)) {
    std::cerr << "Failed to initialize SDL" << std::endl;
    exit(1);
  }
  // Create a window
  window_ = SDL_CreateWindow(name_.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width_, window_height_, SDL_WINDOW_RESIZABLE);

  if (!window_) {
    std::cerr << "Failed to create game window, reason: " << SDL_GetError() << std::endl;
    SDL_Quit();
    exit(1);
  }
  // Create a renderer
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

void
Game::Play() noexcept {
  SDL_Event event;
  bool running = true;
  // start the game loop
  while (running) {
    // query for event
    while (SDL_PollEvent(&event)) {
      // check event type
      switch (event.type) {
        case SDL_QUIT:

          running = false;
          break;

        case SDL_KEYDOWN:
          // check which key has been pressed
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

              for (auto& enemy : enemies_)
                enemy->moveUp(10);
              break;

            case SDLK_s:
            case SDLK_DOWN:

              for (auto& enemy : enemies_)
                enemy->moveDown(10);
              break;

            case SDLK_ESCAPE:

              running = false;
              break;

            default:
              break;
          }
          break;
      }
      // Render all changes to the window
      SDL_RenderClear(renderer_);

      for (auto& player : players_)
        player->updateTexturePos(renderer_);

      for (auto& enemy : enemies_)
        enemy->updateTexturePos(renderer_);

      SDL_RenderPresent(renderer_);
    }
  }
}