#pragma once

#include "../../include/character.hpp"

class Enemy final : public Character {
  public:  
    Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);

  void animateSprite(SDL_Renderer* renderer) noexcept override;
  void moveLeft(int px_step) override;
  void moveRight(int px_step) override;
  void moveUp(int px_step) override;
  void moveDown(int px_step) override;

private:
    static inline size_t s_curr_frame_idx = 0;

};