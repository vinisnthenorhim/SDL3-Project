#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG

#include <loader/image/BWimage.h>
#include "texture.h"

void Texture::createTexture( SDL_GPUDevice* device )
{
  this->device = device;
}

void Texture::loadTexture(const char* path, SDL_GPUCopyPass* copypass)
{
  if (loaded) return;
  loaded = true;

  BW::Image image = BW::imageLoader("../src/assets/image/atlas.png", 4);

  SDL_GPUTextureCreateInfo textureInfo{};
  textureInfo.type = SDL_GPU_TEXTURETYPE_2D;
  textureInfo.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM;
  textureInfo.usage = SDL_GPU_TEXTUREUSAGE_SAMPLER;
  textureInfo.width = image.w;
  textureInfo.height = image.h;
  textureInfo.layer_count_or_depth = 1;
  textureInfo.num_levels = 1;

  transferBuffer.createTransferBuffer(device, image.w * image.h * 4);

  void* mapped = SDL_MapGPUTransferBuffer(device, transferBuffer.buffer(), false);
  memcpy(mapped, image.image, image.w * image.h * 4);
  SDL_UnmapGPUTransferBuffer(device, transferBuffer.buffer());


  texture = SDL_CreateGPUTexture(device, &textureInfo);
  CHECK(texture);

  SDL_GPUTextureTransferInfo src{};
  src.offset = 0;
  src.transfer_buffer = transferBuffer.buffer();

  SDL_GPUTextureRegion dst = {};
  dst.texture = texture;
  dst.w = image.w;
  dst.h = image.h;
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