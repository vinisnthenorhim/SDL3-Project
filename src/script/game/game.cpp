#include "_game.h"

Game::Game(Renderer& renderer, SDL_Window* window)
:camera(window)
{
  this->window = window;
  renderer.dataLoad(world.readData(), world.getObjectTranform(), camera.getCameraPosition());
}

void Game::render(Renderer& renderer)
{

  world.secondPerFrame();
  world.velocityUpdate();
  camera.deltaTime();
  camera.cameraHandle(window);

}

void Game::event( const SDL_Event* event)
{
  camera.setSettings(event);
  if (event->key.key == SDLK_Q )
  {
    world.create(window);
  }
  
}