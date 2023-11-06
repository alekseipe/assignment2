#pragma once

#include <SDL2/SDL.h>
#include <string>

class GameObject {
  public:
    virtual ~GameObject();

    void
    updateTexturePos(SDL_Renderer *renderer) noexcept;

  protected:
    GameObject(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path);

  protected:
    SDL_Rect texture_rect_;
    SDL_Texture *texture_;
    int window_w_, window_h_;
};