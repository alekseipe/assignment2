#include "include/projectile.hpp"
#include <SDL2/SDL_image.h>

Projectile::Projectile(SDL_Renderer* renderer, int x, int y) {
    SDL_Surface* surface = IMG_Load("./src/textures/shoot.png");
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect_.x = x;
    rect_.y = y;
    rect_.w = 37; // Set the width
    rect_.h = 8; // Set the height
}

Projectile::~Projectile() {
    SDL_DestroyTexture(texture_);
}

void Projectile::move(int speed) {
    rect_.x += speed;
}

void Projectile::render(SDL_Renderer* renderer) {
    SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
}

bool Projectile::isOffScreen(int screen_width, int screen_height) {
    return (rect_.y < 0 || rect_.y > screen_height || rect_.x < 0 || rect_.x > screen_width);
}

SDL_Rect Projectile::getRect() const {
    return rect_;
}
