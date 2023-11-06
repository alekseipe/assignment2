#pragma once

#include "../../include/character.hpp"

class Player final : public Character {
  public:
    Player(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);
};