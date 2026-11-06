#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
}
void Game::loadData(CameraDataRW cameraDataRW)
{
  separation.loadData(world.readWriteData(), world.dirtyData(), cameraDataRW);
}
void Game::render()
{
  if(run) world.create(window);

  world.frameCalculate();
  world.velocityUpdate();

  if ( rate[0].tick(20, 60) )  separation.updateCell();

  if ( rate[1].tick(20, 60) ) separation.checkCollide();

  if ( rate[2].tick(60, 60) ) separation.doAnimate();

}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q && !event->key.repeat && event->type == SDL_EVENT_KEY_DOWN)
  {
    run = !run;
  }
  if (event->key.key == SDLK_Y && !event->key.repeat && event->type == SDL_EVENT_KEY_DOWN)
  {
    (*world.readWriteData().speedX)[(*world.readWriteData().playerId)] = 300.0f;
    (*world.readWriteData().speedY)[(*world.readWriteData().playerId)] = 300.0f;
    (*world.readWriteData().playerId)++;
    (*world.readWriteData().speedX)[(*world.readWriteData().playerId)] = 1000.0f;
    (*world.readWriteData().speedY)[(*world.readWriteData().playerId)] = 1000.0f;
  }
}