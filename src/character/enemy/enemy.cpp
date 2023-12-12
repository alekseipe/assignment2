#include "include/enemy.hpp"
#include <SDL2/SDL_image.h>

Enemy::Enemy(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path)
  : Character(window, renderer, std::move(texture_rect), std::move(texture_path)) {
    death_texture_ = IMG_LoadTexture(renderer, "./src/textures/SkeletonKingLeftDeath.png");

}


static Uint32 last_frame_time1 = 0;

const Uint32 frame_delay = 100;
Uint32 last_move_time = 0;
const Uint32 move_delay = 250;
void Enemy::animateSprite(SDL_Renderer* renderer) noexcept
{
    // Get the current time
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_frame_time1 > frame_delay) {
        // Update the frame index
        s_curr_frame_idx = (s_curr_frame_idx + 1) % 10;
        // Reset the last frame time
        last_frame_time1 = current_time;
    }


    // Set up the source rectangle for the current frame
    SDL_Rect src_rect;
    src_rect.x = 64 * s_curr_frame_idx;
    src_rect.y = 0;
    src_rect.w = 64;
    src_rect.h = 64;


    // Render the current frame
    SDL_RenderCopy(renderer, texture_, &src_rect, &texture_rect_);
}

void Enemy::updateMoveTimer(int pix) {
    Uint32 current_time = SDL_GetTicks();
    if (current_time - last_move_time > move_delay) {
        Character::moveLeft(1);
        last_move_time = current_time;  // Reset the timer
    }
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

void Enemy::death(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, death_texture_, nullptr, &texture_rect_);

    // You could add a delay here to display the texture for a set time
    SDL_Delay(1); // Display death animation for 500 milliseconds

    // Flag for deletion
    isDead = true;
}
