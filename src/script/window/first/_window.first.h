#ifndef WINDOWF_H
#define WINDOWF_H
#include <SDL3/SDL.h>
#include <iostream>
class WindowF
{
    public :

        bool create(const char* title);
        bool WindowEventHandler(const SDL_Event* event);

        SDL_Window* getWindow();
        void destroy();

        ~WindowF();

    private :
        int width, height;
        int cWidth, cHeight;
        int x, y;

        SDL_Window* window = nullptr;
        SDL_DisplayID displayID = UINT32_MAX;


        const SDL_DisplayMode* mode;

        bool fullscreen = false;
        bool running = true;
};

#endif