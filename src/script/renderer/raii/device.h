#pragma once

#include <SDL3/SDL.h>
#include "check.h"
class Device
{

  public:
    void createDevice(SDL_Window* window, bool debug, SDL_GPUShaderFormat format)
    {
      this->window = window;
      device = SDL_CreateGPUDevice(format , debug, nullptr);
      CHECK(device);
      CHECK(SDL_ClaimWindowForGPUDevice(device, window));
    }
    ~Device()
    {
      if(!device) return;
      {
        SDL_ReleaseWindowFromGPUDevice(device, window);
        SDL_DestroyGPUDevice(device);  
      }
    }
    SDL_GPUDevice* gpuDevice() {return device;}
  private:
    SDL_Window* window = nullptr;
    SDL_GPUDevice* device = nullptr;
};