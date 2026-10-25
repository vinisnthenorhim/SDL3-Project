#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
  movement.loadData(world.getMovementData());
}

void Game::render()
{

  world.secondPerFrame();
  world.velocityUpdate();
  movement.move({},{}, 0, 0);
  movement.ren();


}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  
}