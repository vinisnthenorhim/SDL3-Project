#include <SDL3/SDL.h>
#include <iostream>

#include <script/window/first/_window.first.h>
#include "check.h"
#include <window/border.settings.h>

bool WindowF::create(const char* title)
{

    if (!SDL_Init(SDL_INIT_VIDEO)) return false;


    window = SDL_CreateWindow(title, 800, 450, SDL_WINDOW_RESIZABLE );
    CHECK(window);
    installCustomFrame(window);
    SDL_SetWindowAspectRatio(window, 16.0 / 10.0f, 16.0 / 9.0f);
    SDL_SetWindowMinimumSize(window, 800, 450);
    SDL_SetWindowBordered(window, true);

    displayID = SDL_GetDisplayForWindow(window);
    mode = SDL_GetDesktopDisplayMode(displayID);
    this->width  = mode->w;
    this->height = mode->h;

    SDL_GL_SetSwapInterval(1);
    
    return window != nullptr ;    
}

bool WindowF::WindowEventHandler(const SDL_Event* event)
{
  fullscreen = SDL_GetWindowProperties(window) & SDL_WINDOW_FULLSCREEN;
  borderFocus(window, event);
    char buffer[256];

    SDL_GetEventDescription(event, buffer, sizeof(buffer));


    switch(event->type)
    {
        case SDL_EVENT_QUIT: 
            running = false; 
            break;
    }

    if(event->key.key == SDLK_F11 && event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
    {
      maximizeOrRestore(window);
    }
    if (event->key.key == SDLK_ESCAPE  && event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
    {
        running = false; 
    }

    return running;
}


SDL_Window* WindowF::getWindow()
{
    return window;
}


void WindowF::destroy()
{
    if (window)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}

WindowF::~WindowF()
{
    destroy();
}