#ifndef GAME_H
#define GAME_H

#include <game/creation/_world.h>

class Game
{
  public:
    Game(SDL_Window* window);
    void render();
    void event(const SDL_Event* event);
    void update();

    ObjectData readData() {return world.readData();};
    ObjectTransformData getObjectTranform() {return world.getObjectTranform();};

  private:
    SDL_Window* window = nullptr;
    const SDL_Event* eventH;
    World world;
};

#endif