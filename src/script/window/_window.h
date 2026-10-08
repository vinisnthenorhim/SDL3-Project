#ifndef WINDOW_H
#define WINDOW_H

#include <SDL3/SDL.h>
#include <iostream>

#include <window/first/_window.first.h>

struct WindowStruct
{
    WindowF windowFirst;
};


class Window
{
    public :
        Window();
        bool event  (const SDL_Event* event);
        bool render ();
        ~Window();

        SDL_Window* getWindowFirst();
        SDL_Window* getWindowSecond();

    private :
        WindowStruct* window;

        WindowF* WindowFirst;

};

#endif