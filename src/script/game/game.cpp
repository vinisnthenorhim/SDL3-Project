#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
}

void Game::render()
{

  world.secondPerFrame();
  world.velocityUpdate();


}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  
}