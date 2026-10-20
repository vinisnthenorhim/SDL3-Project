#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include <stb/stb_image.h>

void Texture::createTexture( SDL_GPUDevice* device )
{
  this->device = device;
}

void Texture::loadTexture(const char* path, SDL_GPUCopyPass* copypass)
{
  if (loaded) return;
  loaded = true;

  int width, height, ch;
  unsigned char* imgLoad = stbi_load("../src/assets/image/player.png", &width, &height, &ch, 4);

  SDL_Log("file channels = %d", ch); 
  
  bool hasAlpha = false;
  for (int i = 0; i < width * height; i++) {
      if (imgLoad[i * 4 + 3] < 255) { hasAlpha = true; break; }
  }
  SDL_Log("has transparent pixels: %d", (int)hasAlpha);

  SDL_GPUTextureCreateInfo textureInfo{};
  textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
  textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
  textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
  textureInfo.width = width;
  textureInfo.height = height;
  textureInfo.layer_count_or_depth = 1;
  textureInfo.num_levels = 1;

  transferBuffer.createTransferBuffer(device, width * height * 4);

  void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer.buffer(), false);
  memcpy(mapped, imgLoad, width * height * 4);
  SDL_UnmapGPUTransferBuffer(device, transferBuffer.buffer());

  texture = SDL_CreateGPUTexture(device, &textureInfo);
  CHECK(texture);

  SDL_GPUTextureTransferInfo src{};
  src.offset = 0;
  src.transfer_buffer = transferBuffer.buffer();

  SDL_GPUTextureRegion dst = {};
  dst.texture = texture;
  dst.w = width;
  dst.h = height;
  dst.d = 1;

  SDL_UploadToGPUTexture(copypass, &src, &dst, false);
}

void Texture::createSampler()
{
  SDL_GPUSamplerCreateInfo si = {};
  si.min_filter     = SDL_GPU_FILTER_NEAREST;
  si.mag_filter     = SDL_GPU_FILTER_NEAREST;
  si.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
  si.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
  sampler = SDL_CreateGPUSampler(device, &si);
  CHECK(sampler);
}

void Texture::setBinding()
{
  binding = {texture, sampler};
}

void Texture::release()
{
  transferBuffer.release();
}

Texture::~Texture()
{
  if (sampler) { SDL_ReleaseGPUSampler(device, sampler); sampler = nullptr; }
  if (texture) { SDL_ReleaseGPUTexture(device, texture); texture = nullptr; }
}