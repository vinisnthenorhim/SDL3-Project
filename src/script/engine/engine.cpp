#include "_engine.h"
#include <filesystem>
#include <iostream>

// #include <chrono>

Engine::Engine()
{
  try
  {
    window = new Window();
    game = new Game( window->getWindowFirst());
    renderer = new Renderer(window->getWindowFirst());
    camera = new Camera(window->getWindowFirst());
    renderer->loadData(game->readOnlyData(), game->dirtyData(), camera->cameraData());
    game->loadData(camera->cameraDataRW());
  }
  catch (const std::exception& err)
  {
    std::cout << "[ENGINE]  [CONSTRUCTOR]\n";
    std::cout << "[ERROR]   " << err.what();
  }
}



bool Engine::event(const SDL_Event* event)
{

  try
  {
    game->event(event);
    camera->cameraEvent(event);
    renderer->screenUpdate(event);
    return window->event(event);
  }
  catch (const std::exception& err)
  {
    std::cout << "[ENGINE]  [EVENT]\n";
    std::cout << "[ERROR]   " << err.what();
    return false;
  }
}



bool Engine::render()
{
  try
  {
    game->render();
    camera->itterate();
    // auto t0 = std::chrono::steady_clock::now();
    renderer->render();
    // auto t1 = std::chrono::steady_clock::now();
    // std::cout << "name: " << std::chrono::duration<float, std::milli>(t1 - t0).count() << " ms\n";
  }
  catch (const std::exception& err)
  {
    std::cout << "[ENGINE]  [RENDER]\n";
    std::cout << "[ERROR]   " << err.what();
    return false;
  }
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