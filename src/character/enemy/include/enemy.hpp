#pragma once

#include "../../include/character.hpp"

class Enemy final : public Character {
  public:  
    Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);
};