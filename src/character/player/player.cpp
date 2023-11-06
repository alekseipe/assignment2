#include "include/player.hpp"

Player::Player(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path)
  : Character(window, renderer, std::move(texture_rect), std::move(texture_path)) {}