#include "_engine.h"
#include <filesystem>
#include <iostream>
Engine::Engine()
{
  window = new Window();
  game = new Game( window->getWindowFirst());
  renderer = new Renderer(window->getWindowFirst());
  camera = new Camera(window->getWindowFirst());
  renderer->dataLoad(game->readData(), game->getObjectTranform(), camera->getCameraPosition());
}



bool Engine::event(const SDL_Event* event)
{
  game->event(event);
  camera->cameraEvent(event);
  renderer->screenUpdate(event);
  return window->event(event);
}



bool Engine::render()
{
  game->render();
  camera->itterate();
  renderer->render();
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