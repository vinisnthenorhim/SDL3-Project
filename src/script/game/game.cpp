#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
}
void Game::loadData(Position cameraPosition)
{
  movement.loadData(world.readData(), world.getMovementData(), cameraPosition);
}
void Game::render()
{

  world.secondPerFrame();
  world.velocityUpdate();
  movement.updateCell();
  movement.move(0,0,0);
  movement.doAnimate();

  world.updateData();
}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  if (event->key.key == SDLK_Y && !event->key.repeat && event->type == SDL_EVENT_KEY_DOWN)
  {
    (*world.getMovementData().speedX)[(*world.readData().playerId)] = 300.0f;
    (*world.getMovementData().speedY)[(*world.readData().playerId)] = 300.0f;
    (*world.readData().playerId)++;
    (*world.getMovementData().speedX)[(*world.readData().playerId)] = 1000.0f;
    (*world.getMovementData().speedY)[(*world.readData().playerId)] = 1000.0f;
  }
}