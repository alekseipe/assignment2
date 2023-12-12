#include "include/character.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

Character::Character(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path)
  : GameObject(window, renderer, std::move(texture_rect), std::move(texture_path)) {}

void
Character::moveLeft(int px_step) {
  texture_rect_.x -= px_step;

  if (texture_rect_.x < 0)
    texture_rect_.x = 0;
}

void
Character::moveRight(int px_step) {
  texture_rect_.x += px_step;

  if (texture_rect_.x + texture_rect_.w > window_w_)
    texture_rect_.x = window_w_ - texture_rect_.w;
}

void
Character::moveUp(int px_step) {
  texture_rect_.y -= px_step;

  if (texture_rect_.y < 0)
    texture_rect_.y = 0;
}

void
Character::moveDown(int px_step) {
  texture_rect_.y += px_step;

  if (texture_rect_.y + texture_rect_.h > window_h_)
    texture_rect_.y = window_h_ - texture_rect_.h;
}

int Character::getX() const {
    return texture_rect_.x;
}

int Character::getWidth() const {
    return texture_rect_.w;
}

void Character::setX(int x) {
    texture_rect_.x = x;
}


SDL_Rect Character::getRect() const {
    return texture_rect_; 
}

void
Character::animateSprite(SDL_Renderer* renderer) noexcept
{}


int Character::getHealth() const {
    return health_;
}

void Character::setHealth(int health) {
    health_ = health;
}

void Character::death(SDL_Renderer* renderer) {
}

int Character::GetImageWidth(const std::string& filePath) {
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

int Character::GetImageHeight(const std::string& filePath) {
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

    // Get the height of the image
    int height = imageSurface->h;

    // Clean up the surface
    SDL_FreeSurface(imageSurface);

    // Quit SDL_image
    IMG_Quit();

    return height;
}