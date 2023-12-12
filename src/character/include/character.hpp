#pragma once

#include "../../game_object/include/game_object.hpp"

class Character : public GameObject {
  public:
    virtual  void
    moveLeft(int px_step);

    virtual void
    moveRight(int px_step);

    virtual  void
    moveUp(int px_step);

    virtual  void
    moveDown(int px_step);

    int getX() const;

    int getWidth() const;

    SDL_Rect getRect() const;

    void setX(int x);

    
    void animateSprite(SDL_Renderer* renderer) noexcept override;
  
  protected:
    Character(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path);
};