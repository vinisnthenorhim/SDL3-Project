#ifndef GAME_H
#define GAME_H

#include <game/creation/_world.h>
#include <logic/separation/separation.h>
#include <logic/grid.h>
#include <logic/grid.h>
#include <logic/player.h>

class Game
{
  private:
    SDL_Window* window = nullptr;
    const SDL_Event* eventH = nullptr;

    World world;
    GridCell cell;
    ObjectSeparation separation;
    Player player;

    bool run = false;

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

};

#endif