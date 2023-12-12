#include "include/Collectable.hpp"
#include <SDL2/SDL_image.h>

Collectable::Collectable(SDL_Renderer* renderer, int x, int y) : collected_(false) {
    SDL_Surface* surface = IMG_Load("./src/textures/heart.png");
    texture_ = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    rect_.x = x;
    rect_.y = y;
    rect_.w = 16;
    rect_.h = 16; 
}

Collectable::~Collectable() {
    SDL_DestroyTexture(texture_);
}

void Collectable::render(SDL_Renderer* renderer) {
    if (!collected_) {
        SDL_RenderCopy(renderer, texture_, nullptr, &rect_);
    }
}

SDL_Rect Collectable::getRect() const {
    return rect_;
}

bool Collectable::isCollected() const {
    return collected_;
}

void Collectable::collect() {
    collected_ = true;
}

SDL_Rect* Collectable::getRect() {
    return &rect_;
}