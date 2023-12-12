#pragma once

#include "../../include/character.hpp"

class Enemy final : public Character {
  public:  
    Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);

  void animateSprite(SDL_Renderer* renderer) noexcept override;
  void updateMoveTimer(int pix);
  void moveLeft(int px_step) override;
  void moveRight(int px_step) override;
  void moveUp(int px_step) override;
  void moveDown(int px_step) override;
  void death(SDL_Renderer* renderer) override;

private:
    static inline size_t s_curr_frame_idx = 0;
    SDL_Texture* death_texture_;

};