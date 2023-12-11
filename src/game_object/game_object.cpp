#include "include/game_object.hpp"
#include <SDL2/SDL_image.h>
#include <iostream>

GameObject::GameObject(SDL_Window *window, SDL_Renderer *renderer, SDL_Rect &&texture_rect, std::string &&texture_path) : texture_rect_(std::move(texture_rect)) {
  // load image and convert it to texture
  texture_ = IMG_LoadTexture(renderer, texture_path.data());

  if (!texture_) {
    std::cerr << "Failed to load a texture from " << texture_path << ", probably the file doesn't exist" << std::endl;
    exit(1);
  }
  // fetch window resolution
  SDL_GetWindowSize(window, &window_w_, &window_h_);
}

GameObject::~GameObject() {
  SDL_DestroyTexture(texture_);
}

void
GameObject::updateTexturePos(SDL_Renderer *renderer) noexcept {
  SDL_RenderCopy(renderer, texture_, nullptr, &texture_rect_);
}

