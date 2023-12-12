#include "include/enemy.hpp"

Enemy::Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path)
  : Character(window, renderer, std::move(texture_rect), std::move(texture_path)) {}
Uint32 last_frame_time = 0;
const Uint32 frame_delay = 100;
void Enemy::animateSprite(SDL_Renderer* renderer) noexcept
{
    // Get the current time
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_frame_time > frame_delay) {
        // Update the frame index
        s_curr_frame_idx = (s_curr_frame_idx + 1) % 6;
        // Reset the last frame time
        last_frame_time = current_time;
    }


    // Set up the source rectangle for the current frame
    SDL_Rect src_rect;
    src_rect.x = 48 * s_curr_frame_idx;
    src_rect.y = 0;
    src_rect.w = 48;
    src_rect.h = 48;


    // Render the current frame
    SDL_RenderCopy(renderer, texture_, &src_rect, &texture_rect_);
}


void Enemy::moveLeft(int px_step) {
    Character::moveLeft(px_step);
   // current_texture_ = textures_["left"];
   // last_direction_ = "left";
  //  moving_ = true;
}

void Enemy::moveRight(int px_step) {
    Character::moveRight(px_step);

}

void Enemy::moveUp(int px_step) {
    Character::moveUp(px_step);

}

void Enemy::moveDown(int px_step) {
    Character::moveDown(px_step);

}
