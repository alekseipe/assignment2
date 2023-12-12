#include <SDL2/SDL.h>

class Collectable {
public:
    Collectable(SDL_Renderer* renderer, int x, int y);
    ~Collectable();

    void render(SDL_Renderer* renderer);
    SDL_Rect getRect() const;
    bool isCollected() const;
    void collect();
    SDL_Rect* getRect();

private:
    SDL_Texture* texture_;
    SDL_Rect rect_;
    bool collected_;
};

