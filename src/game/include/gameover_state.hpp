#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>

class GameOverState {
public:
    GameOverState(SDL_Renderer* renderer);
    ~GameOverState();

    void HandleEvents(SDL_Event& event, bool& running, bool& restart);
    void Render();

private:
    SDL_Renderer* renderer_;
    TTF_Font* font_;
    SDL_Texture* messageTexture_;
    SDL_Rect messageRect_;
};
