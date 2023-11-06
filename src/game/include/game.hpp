#pragma once

#include "../../character/include/character.hpp"
#include "../../character/player/include/player.hpp"
#include "../../character/enemy/include/enemy.hpp"

#include <SDL2/SDL.h>
#undef main
#include <string>
#include <vector>
#include <type_traits>
#include <memory>

class Game final {
  public:
    Game(const std::string name, int window_width, int window_height);
    ~Game();

    template <typename CharType>
    void
    AddCharacter(int w, int h, int x, int y, std::string texture_path);

    void
    Play() noexcept;

  private:
    const std::string name_;
    const int window_height_, window_width_;
    std::vector<std::unique_ptr<Character>> players_, enemies_;
    SDL_Window *window_;
    SDL_Renderer *renderer_;
};

template <typename CharType>
void
Game::AddCharacter(int w, int h, int x, int y, std::string texture_path) {
  // statically check if an appropriate object type has been provided
  static_assert(std::is_base_of_v<Character, CharType>, "The provided class is not derived from Character class");

  SDL_Rect texture_rect;
  texture_rect.w = w;
  texture_rect.h = h;
  texture_rect.x = x;
  texture_rect.y = y;

  if constexpr (std::is_same_v<CharType, Player>)
    players_.push_back(std::make_unique<CharType>(window_, renderer_, texture_rect, texture_path));
  else
    enemies_.push_back(std::make_unique<CharType>(window_, renderer_, texture_rect, texture_path));
}