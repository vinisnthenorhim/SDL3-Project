#include "_engine.h"
#include <filesystem>
#include <iostream>
Engine::Engine()
{
  window = new Window();
  renderer = new Renderer(window->getWindowFirst());
  game = new Game( window->getWindowFirst());
  camera = new Camera(window->getWindowFirst());

  renderer->dataLoad(game->readData(), game->getObjectTranform(), camera->getCameraPosition());
}



bool Engine::event(const SDL_Event* event)
{
  game->event(event);
  renderer->screenUpdate(event);
  camera->cameraEvent(event);
  return window->event(event);
}



bool Engine::render()
{
  game->render();
  renderer->render();
  camera->itterate();
  return true;
}



Engine::~Engine()
{
  delete camera;
  camera = nullptr;
  delete game;
  game = nullptr;
  delete renderer;
  renderer = nullptr;
  delete window;
  window = nullptr;
}