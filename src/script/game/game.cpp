#include "_game.h"

Game::Game(Renderer& renderer, SDL_Window* window)
{
  this->window = window;
  world.createObject(Type::Player, 0, 0);
  renderer.dataLoad(world.readData(), world.getObjectTranform(), camera.getCameraPosition());
}

void Game::render(Renderer& renderer)
{

  world.secondPerFrame();
  world.velocityUpdate();
  camera.deltaTime();
  camera.cameraHandle();
  camera.movingWindow(window, eventH);

}

void Game::event( const SDL_Event* event)
{
  this->eventH = event;
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  
}