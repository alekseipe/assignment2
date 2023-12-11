#pragma once

#include "../../include/character.hpp"

class Player final : public Character {
  public:
    Player(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);

  void animateSprite(SDL_Renderer* renderer) noexcept override;

private:
    static inline size_t s_curr_frame_idx = 0;
};