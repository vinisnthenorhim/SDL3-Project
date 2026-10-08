#include "_window.h"

Window::Window()
{
    window = new WindowStruct();
    WindowFirst = &window->windowFirst;

    if(!WindowFirst->create(" ")) 
    {
        std::cerr << "Fail at Window Manager create window" << '\n';
    }
}

bool Window::event(const SDL_Event* event)
{
    return WindowFirst->WindowEventHandler(event);
}

bool Window::render()
{
    return true;
}

Window::~Window()
{

    WindowFirst->destroy();

    delete window;
    window = nullptr;

}


SDL_Window* Window::getWindowFirst()
{
    return WindowFirst->getWindow();
}