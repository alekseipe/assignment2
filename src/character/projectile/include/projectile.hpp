#include <SDL2/SDL.h>

class Projectile {
public:
    Projectile(SDL_Renderer* renderer, int x, int y);
    ~Projectile();

    void move(int speed);
    void render(SDL_Renderer* renderer);
    bool isOffScreen(int screen_width, int screen_height);
    SDL_Rect getRect() const;
private:
    SDL_Texture* texture_;
    SDL_Rect rect_;
};
