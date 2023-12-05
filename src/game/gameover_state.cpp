// GameOverState.cpp
#include "include/gameover_state.hpp"
#include <iostream>

GameOverState::GameOverState(SDL_Renderer* renderer) : renderer_(renderer), font_(nullptr), messageTexture_(nullptr) {
    // Initialize SDL_ttf
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
    }

    // Load font
    font_ = TTF_OpenFont("./src/textures/daisy-roots.ttf", 28); // Replace with your font path and size
    if (font_ == nullptr) {
        std::cerr << "Failed to load font! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create text surface
    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Surface* textSurface = TTF_RenderText_Solid(font_, "Game Over. Press R to Restart", textColor);
    if (textSurface == nullptr) {
        std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
        return;
    }

    messageTexture_ = SDL_CreateTextureFromSurface(renderer_, textSurface);
    messageRect_ = { 100, 100, textSurface->w, textSurface->h }; // Adjust position and size

    SDL_FreeSurface(textSurface);
}


GameOverState::~GameOverState() {
    if (font_ != nullptr) {
        TTF_CloseFont(font_);
    }
    if (messageTexture_ != nullptr) {
        SDL_DestroyTexture(messageTexture_);
    }
    TTF_Quit();
}

void GameOverState::HandleEvents(SDL_Event& event, bool& running, bool& restart) {
    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_r) {
            restart = true;
        }
    }
}

void GameOverState::Render() {
    SDL_RenderCopy(renderer_, messageTexture_, nullptr, &messageRect_);
}
