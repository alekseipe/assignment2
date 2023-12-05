#pragma once

#include "../../game_object/include/game_object.hpp"

class Character : public GameObject {
  public:
    void
    moveLeft(int px_step);

    void
    moveRight(int px_step);

    void
    moveUp(int px_step);

    void
    moveDown(int px_step);

    int getX() const;

    int getWidth() const;

    SDL_Rect getRect() const;

    void setX(int x);
  
  protected:
    Character(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path);
};