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

    virtual int getHealth() const;

    virtual void setHealth(int health);

    virtual void death(SDL_Renderer* renderer);

    bool isDead = false;

    void animateSprite(SDL_Renderer* renderer) noexcept override;
  
    private:
     int health_;

  protected:
    Character(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path);
};