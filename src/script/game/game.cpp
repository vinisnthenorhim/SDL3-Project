#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
  movement.loadData(world.getMovementData());
  for (int a = 0; a < 10000; a++)
  {
    world.create(window);
  }
}

void Game::render()
{

  world.secondPerFrame();
  world.velocityUpdate();
  if (chase)
  {
    movement.move();
  }
  else
  {
    movement.move( true );
  }


  world.updateData();
}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  if (event->key.key == SDLK_Z && !event->key.repeat && event->type == SDL_EVENT_KEY_DOWN)
  {
    chase = !chase;
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