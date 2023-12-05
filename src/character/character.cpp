#include "include/character.hpp"

Character::Character(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path)
  : GameObject(window, renderer, std::move(texture_rect), std::move(texture_path)) {}

void
Character::moveLeft(int px_step) {
  texture_rect_.x -= px_step;

  if (texture_rect_.x < 0)
    texture_rect_.x = 0;
}

void
Character::moveRight(int px_step) {
  texture_rect_.x += px_step;

  if (texture_rect_.x + texture_rect_.w > window_w_)
    texture_rect_.x = window_w_ - texture_rect_.w;
}

void
Character::moveUp(int px_step) {
  texture_rect_.y -= px_step;

  if (texture_rect_.y < 0)
    texture_rect_.y = 0;
}

void
Character::moveDown(int px_step) {
  texture_rect_.y += px_step;

  if (texture_rect_.y + texture_rect_.h > window_h_)
    texture_rect_.y = window_h_ - texture_rect_.h;
}

int Character::getX() const {
    return texture_rect_.x;
}

int Character::getWidth() const {
    return texture_rect_.w;
}

void Character::setX(int x) {
    texture_rect_.x = x;
}

SDL_Rect Character::getRect() const {
    return texture_rect_; // Assuming texture_rect_ is the SDL_Rect representing the character
}
