#ifndef ENGINE_H
#define ENGINE_H

#include <SDL3/SDL.h>

#include <window/_window.h>
#include <renderer/_renderer.h>
#include <game/_game.h>
#include <camera/camera.h>


class Engine
{
  public:
    Engine();
    
    bool event  (const SDL_Event* event);
    bool render ();

    ~Engine();

  private:
    Window* window = nullptr;
    Renderer* renderer = nullptr;
    Game* game = nullptr;
    Camera* camera = nullptr;

};

#endif