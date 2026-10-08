#include "_engine.h"

Engine::Engine()
{
  window = new Window();
  renderer = new Renderer(window->getWindowFirst());
  game = new Game(*renderer);

}



bool Engine::event(const SDL_Event* event)
{

  renderer->screenUpdate(event);
    
  game->event(event);

  return window->event(event);
}

bool Engine::render()
{
  game->render(*renderer);
  renderer->render();
  return true;
}



Engine::~Engine()
{
  delete game;
  game = nullptr;
  delete renderer;
  renderer = nullptr;
  delete window;
  window = nullptr;
}