#include "_engine.h"
#include <filesystem>
#include <iostream>

// #include <chrono>

Engine::Engine()
{
  window = new Window();
  game = new Game( window->getWindowFirst());
  renderer = new Renderer(window->getWindowFirst());
  camera = new Camera(window->getWindowFirst());
  renderer->loadData(game->readOnlyData(), game->dirtyData(), camera->cameraData());
  game->loadData(camera->cameraDataRW());
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
  // auto t0 = std::chrono::steady_clock::now();
  renderer->render();
  // auto t1 = std::chrono::steady_clock::now();
  // std::cout << "name: " << std::chrono::duration<float, std::milli>(t1 - t0).count() << " ms\n";
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