#include "include/player.hpp"
#include <iostream>
#include <SDL2/SDL_image.h>


Player::Player(SDL_Window* window, SDL_Renderer* renderer, SDL_Rect texture_rect, std::string texture_path)
    : Character(window, renderer, std::move(texture_rect), std::move(texture_path)),
    texture_path_(std::move(texture_path)) {


    textures_["left"] = IMG_LoadTexture(renderer, "./src/textures/WarriorLeftWalk.png");
    textures_["right"] = IMG_LoadTexture(renderer, "./src/textures/WarriorRightWalk.png");
    textures_["up"] = IMG_LoadTexture(renderer, "./src/textures/WarriorUpWalk.png");
    textures_["down"] = IMG_LoadTexture(renderer, "./src/textures/WarriorDownWalk.png");

    // Load idle textures
    textures_["left_idle"] = IMG_LoadTexture(renderer, "./src/textures/WarriorLeftIdle.png");
    textures_["right_idle"] = IMG_LoadTexture(renderer, "./src/textures/WarriorRightIdle.png");
    textures_["up_idle"] = IMG_LoadTexture(renderer, "./src/textures/WarriorUpIdle.png");
    textures_["down_idle"] = IMG_LoadTexture(renderer, "./src/textures/WarriorDownIdle.png");

    // Set the current texture to the initial texture
    current_texture_ = textures_["right_idle"];
    setHealth(3);

}

// Add a static variable to keep track of the time since the last frame update
static Uint32 last_frame_time = 0;
// Define the time interval (in milliseconds) between frame updates
const Uint32 frame_delay = 100; // 100 ms between frames

int GetImageWidth(const std::string& filePath) {
    // Initialize SDL_image
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
        return -1;
    }

    // Load the image into an SDL_Surface
    SDL_Surface* imageSurface = IMG_Load(filePath.c_str());
    if (!imageSurface) {
        std::cerr << "IMG_Load Error: " << IMG_GetError() << std::endl;
        IMG_Quit();
        return -1;
    }

    // Get the width of the image
    int width = imageSurface->w;

    // Clean up the surface
    SDL_FreeSurface(imageSurface);

    // Quit SDL_image
    IMG_Quit();

    return width;
}
void Player::animateSprite(SDL_Renderer* renderer) noexcept 
{
    // Get the current time
    Uint32 current_time = SDL_GetTicks();
    int imageWidth = GetImageWidth(texture_path_);
    int amountOfFrames = imageWidth / 48;

    // Check if enough time has passed since the last frame update
    if (current_time - last_frame_time > frame_delay) {
        // Update the frame index
        s_curr_frame_idx = (s_curr_frame_idx + 1) % 4;
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
    SDL_RenderCopy(renderer, current_texture_, &src_rect, &texture_rect_);
}

void Player::moveLeft(int px_step) {
    Character::moveLeft(px_step);
    current_texture_ = textures_["left"];
    last_direction_ = "left";
    moving_ = true;
}

void Player::moveRight(int px_step) {
    Character::moveRight(px_step);
    current_texture_ = textures_["right"];
    last_direction_ = "right";
    moving_ = true;
}

void Player::moveUp(int px_step) {
    Character::moveUp(px_step);
    current_texture_ = textures_["up"];
    last_direction_ = "up";
    moving_ = true;
}

void Player::moveDown(int px_step) {
    Character::moveDown(px_step);
    current_texture_ = textures_["down"];
    last_direction_ = "down";
    moving_ = true;
}

void Player::setIdleState() {
    if (last_direction_ == "left") {
        current_texture_ = textures_["left_idle"];
    }
    else if (last_direction_ == "right") {
        current_texture_ = textures_["right_idle"];
    }
    else if (last_direction_ == "up") {
        current_texture_ = textures_["up_idle"];
    }
    else if (last_direction_ == "down") {
        current_texture_ = textures_["down_idle"];
    }
}

bool Player::isMoving() const {
    return moving_;
}

void Player::stopMoving() {
    moving_ = false;
    setIdleState();
}
int Player::getHealth() const {
    return health_;
}

void Player::setHealth(int health) {
    health_ = health;
}

void Player::shoot(SDL_Renderer* renderer) {
    int projectileStartX = texture_rect_.x + texture_rect_.w; // Right side of the player
    int projectileStartY = texture_rect_.y + texture_rect_.h / 2; // Vertically centered

    projectiles_.push_back(new Projectile(renderer, projectileStartX, projectileStartY));
}

void Player::updateProjectiles() {
    for (auto it = projectiles_.begin(); it != projectiles_.end();) {
        (*it)->move(5); // Adjust speed as needed
        if ((*it)->isOffScreen(window_w_, window_h_)) {
            delete* it;
            it = projectiles_.erase(it);
        }
        else {
            ++it;
        }
    }
}

void Player::renderProjectiles(SDL_Renderer* renderer) {
    for (auto& projectile : projectiles_) {
        projectile->render(renderer);
    }
}