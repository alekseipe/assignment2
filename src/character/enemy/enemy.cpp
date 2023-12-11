#include "include/enemy.hpp"

Enemy::Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path)
  : Character(window, renderer, std::move(texture_rect), std::move(texture_path)) {}

//void
//Enemy::animateSprite(SDL_Renderer* renderer) noexcept;