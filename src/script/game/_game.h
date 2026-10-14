#ifndef GAME_H
#define GAME_H

#include <renderer/_renderer.h>
#include <game/creation/_world.h>
class Game
{
  public:
    Game(Renderer& renderer, SDL_Window* window);
    void render(Renderer& renderer);
    void event(const SDL_Event* event);
    void update();

  private:
    SDL_Window* window = nullptr;
    World world;
};

#endif