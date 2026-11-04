#ifndef GAME_H
#define GAME_H

#include <game/creation/_world.h>
#include <logic/movement/movement.h>

class Game
{
  public:
    Game(SDL_Window* window);
    void render();
    void event(const SDL_Event* event);
    void update();
    void loadData(CameraDataRW cameraDataRW);

    ObjectDataRO readOnlyData() {return world.readOnlyData() ; }
    ObjectDataRW readWriteData(){return world.readWriteData(); }
    DirtyData    dirtyData()    {return world.dirtyData()    ; }

    ~Game()
    {
      world.saveData();
    }
  private:
    SDL_Window* window = nullptr;
    const SDL_Event* eventH = nullptr;
    World world;
    ObjectSeparation separation;
};

#endif