#include "include/player.hpp"
#include <iostream>

Player::Player(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect texture_rect, std::string texture_path)
  : Character(window, renderer, std::move(texture_rect), std::move(texture_path)) {}
// Add a static variable to keep track of the time since the last frame update
static Uint32 last_frame_time = 0;
// Define the time interval (in milliseconds) between frame updates
const Uint32 frame_delay = 100; // 100 ms between frames

void Player::animateSprite(SDL_Renderer* renderer) noexcept 
{
    // Get the current time
    Uint32 current_time = SDL_GetTicks();

    // Check if enough time has passed since the last frame update
    if (current_time - last_frame_time > frame_delay) {
        // Update the frame index
        s_curr_frame_idx = (s_curr_frame_idx + 1) % 8;
        // Reset the last frame time
        last_frame_time = current_time;
    }
    std::cout << texture_rect_.w << std::endl;

    // Set up the source rectangle for the current frame
    SDL_Rect src_rect;
    src_rect.x = 48 * s_curr_frame_idx;
    src_rect.y = 0;
    src_rect.w = 48;
    src_rect.h = 48;

    
    // Render the current frame
    SDL_RenderCopy(renderer, texture_, &src_rect, &texture_rect_);
}
