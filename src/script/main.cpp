#define SDL_MAIN_USE_CALLBACKS 1

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <iostream>
#include <stdexcept>

#include <script/engine/_engine.h>


SDL_AppResult SDL_AppInit (void** appstate, int argc, char *argv[])
{

    freopen("log.txt", "w", stdout);
    freopen("log.txt", "a", stderr);

    Engine* Chaoss = new Engine();
    *appstate = Chaoss;

    return SDL_APP_CONTINUE;

}

SDL_AppResult SDL_AppEvent (void* appstate, SDL_Event* event)
{

    Engine* Chaoss = static_cast<Engine*>(appstate);
    if(!Chaoss->event(event)) return SDL_APP_SUCCESS;

    return SDL_APP_CONTINUE;

}

SDL_AppResult SDL_AppIterate (void* appstate)
{

    Engine* Chaoss = static_cast<Engine*>(appstate);
    if(!Chaoss->render()) return SDL_APP_FAILURE;

    return SDL_APP_CONTINUE;

}


void SDL_AppQuit (void* appstate, SDL_AppResult result)
{

    Engine* Chaoss = static_cast<Engine*>(appstate);
    if (appstate)
    {

        delete Chaoss;

        Chaoss = nullptr;
    }
    SDL_Quit();
    
}