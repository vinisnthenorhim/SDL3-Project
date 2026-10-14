#include "_game.h"

Game::Game(Renderer& renderer, SDL_Window* window)
{
  this->window = window;
  world.createObject(Type::Player, 0, 0);
  renderer.dataLoad(world.readData(), world.getObjectTranform());
}

void Game::render(Renderer& renderer)
{

  world.secondPerFrame();
  // const int& size = world.dirtyObject().size();
  // for(int a = 0; a < size; a++)
  // {
  //   if (world.dirtyObject()[a])
  //   {
  //     renderer.render();
  //   }
  // }
  // const bool* keyboardState = SDL_GetKeyboardState(NULL);
  world.velocityUpdate();

}

void Game::event(const SDL_Event* event)
{
  if (event->key.key == SDLK_Q && event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
  {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    world.create(width, height);
  }
  
}