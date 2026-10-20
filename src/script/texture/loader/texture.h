#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL.h>
#include <renderer/raii/_raii.h>
class Texture
{
  private:
    SDL_GPUTexture* texture = nullptr;
    TransferBuffer transferBuffer;
    SDL_GPUSampler* sampler = nullptr;
    SDL_GPUDevice* device = nullptr;
    SDL_GPUTextureSamplerBinding binding {};
    bool loaded = false;

  public:
    void createTexture( SDL_GPUDevice* device );
    void loadTexture(const char* path, SDL_GPUCopyPass* copypass);
    void createSampler();
    void setBinding();
    void release();
    SDL_GPUTextureSamplerBinding getBinding () { return binding; }

    ~Texture();
};