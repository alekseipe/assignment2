#pragma once

#include "../../include/character.hpp"
#include <map>

class Player final : public Character {
  public:
    Player(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path);

  void animateSprite(SDL_Renderer* renderer) noexcept override;


  // Override movement methods
  void moveLeft(int px_step) override;
  void moveRight(int px_step) override;
  void moveUp(int px_step) override;
  void moveDown(int px_step) override;
  bool isMoving() const;
  void setIdleState();
  void stopMoving();



private:
    static inline size_t s_curr_frame_idx = 0;
    std::string texture_path_;
    std::map<std::string, SDL_Texture*> textures_; // Map to store textures
    SDL_Texture* current_texture_; // Current texture in use
    std::string last_direction_; // Add this member to track the last direction
    bool moving_;
};