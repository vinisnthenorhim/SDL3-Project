#include "_engine.h"

Engine::Engine()
{
  window = new Window();
  renderer = new Renderer(window->getWindowFirst());
  game = new Game(*renderer);
  renderer->transfer();
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
  if(renderer->copy()) renderer->render();
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