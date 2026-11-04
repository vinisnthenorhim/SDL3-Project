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

  world.frameCalculate();
  world.velocityUpdate();

  separation.updateCell();

  separation.checkCollide();

  separation.doAnimate();

}

void Game::event( const SDL_Event* event)
{
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
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