#ifndef GAME_H
#define GAME_H

#include <renderer/_renderer.h>
#include <game/creation/_world.h>
class Game
{
  public:
    Game(Renderer& renderer);
    void render(Renderer& renderer);
    void event(const SDL_Event* event);
    void update();

  private:
    // bool update = false;
    World world;
};

#endif