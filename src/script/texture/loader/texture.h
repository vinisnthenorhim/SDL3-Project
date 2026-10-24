#pragma once

#include <vector>
#include <SDL3/SDL.h>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>
#include <SDL3/SDL.h>
#include <renderer/raii/_raii.h>
#include <game/entity/entity.h>
#include <struct/struct.h>

struct AtlasDimension
{
  float h;
  float w;
};

struct SpriteDimension
{
  float x;
  float y;
  float w;
  float h;
};

class Texture
{
  private:
    Buffer layout;
    Buffer textureIDBuffer;
    TransferBuffer layoutTrans;
    TransferBuffer atlasTransferBuffer;
    TransferBuffer transferBuffer;

    SDL_GPUTexture* texture = nullptr;
    SDL_GPUSampler* sampler = nullptr;
    SDL_GPUDevice* device = nullptr;
    SDL_GPUTextureSamplerBinding binding {};

    std::vector<AtlasDimension>  atlasDimension{};
    std::vector<SpriteDimension> spDimension   {};
    std::vector<int>*   type         ;

    Uint32 lastCount = 0;

    bool loaded = false;

    Entity entity;

  public:
    Texture();
    void createTexture( SDL_GPUDevice* device );
    void updateTextureIdBUffer( SDL_GPUCopyPass* copypass );
    void loadTexture( SDL_GPUCopyPass* copypass);
    void loadData(ObjectData data);
    void createSampler();
    void setBinding();
    void release();
    void bindSamplers(SDL_GPURenderPass* pass, uint32_t slot);
    void bindLayout(SDL_GPURenderPass* pass, uint32_t slot);
    SDL_GPUTextureSamplerBinding getBinding () { return binding; }
    void getAtlasData(SDL_GPUCopyPass* copypass);

    ~Texture();
};