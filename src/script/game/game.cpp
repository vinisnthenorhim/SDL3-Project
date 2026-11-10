#include "_game.h"

Game::Game( SDL_Window* window)
{
  this->window = window;
}
void Game::loadData(CameraDataRW cameraDataRW)
{
  cell.loadData(world.readOnlyData(), cameraDataRW);
  separation.loadData(world.readWriteData(), world.dirtyData(), cell.getCell());
  pathFinding.loadData(world.readWriteData(), world.dirtyData());

  auto func = [this](float x, float y, float targetX, float targetY, uint64_t id)
  {
    pathFinding.findPath(x, y, targetX, targetY, id);
  };
  player.loadData(world.readWriteData(), world.dirtyData(), window, cameraDataRW, func);
}
void Game::render()
{
  if (run) world.create(window);

  world.frameCalculate();
  player.movement();

  pathFinding.animate();
  cell.updateCell();

  separation.updateSize();

  separation.checkCollide();

  separation.doAnimate();

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