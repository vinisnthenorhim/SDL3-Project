#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <loader/image/BWimage.h>
#include "texture.h"

Texture::Texture()
{
  BW::File file = {"../src/data/json/texture/atlas.json", "rb"};

  char buf[16384];
  rapidjson::FileReadStream is(file.fp, buf, sizeof(buf));

  rapidjson::Document entityFile;
  entityFile.ParseStream(is);
}

void Texture::createTexture( SDL_GPUDevice* device )
{
  this->device = device;
}

void Texture::loadData(ObjectData data)
{
  this->type = data.type;
}

void Texture::loadTexture( SDL_GPUCopyPass* copypass)
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

  atlasTransferBuffer.createTransferBuffer(device, image.w * image.h * 4);

  void* mapped = SDL_MapGPUTransferBuffer(device, atlasTransferBuffer.buffer(), false);
  memcpy(mapped, image.image, image.w * image.h * 4);
  SDL_UnmapGPUTransferBuffer(device, atlasTransferBuffer.buffer());


  texture = SDL_CreateGPUTexture(device, &textureInfo);
  CHECK(texture);

  SDL_GPUTextureTransferInfo src{};
  src.offset = 0;
  src.transfer_buffer = atlasTransferBuffer.buffer();

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
  layout.releaseOldBuffer();
  textureIDBuffer.releaseOldBuffer();
  atlasTransferBuffer.release();
  layoutTrans.release();
  transferBuffer.release();
}

Texture::~Texture()
{
  release();
  if (sampler) { SDL_ReleaseGPUSampler(device, sampler); sampler = nullptr; }
  if (texture) { SDL_ReleaseGPUTexture(device, texture); texture = nullptr; }
}

void Texture::updateTextureIdBUffer( SDL_GPUCopyPass* copypass )
{
  if (type->size() <= 0) return;

  Uint32 currentCount = type->size();
  Uint32 difference = currentCount - lastCount;
  Uint32 offset = lastCount * sizeof(int);
  std::vector<int> tempIdUpload{};

  for (int a = lastCount; a < currentCount; a++)
  {
    tempIdUpload.push_back((*type)[a]);
  }

  if (!textureIDBuffer.buffer()) textureIDBuffer.createBuffer(device, sizeof(int) * currentCount, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else textureIDBuffer.updateBufferSize(copypass, sizeof(int) * currentCount, true);

  if( !tempIdUpload.empty())
  {
    lastCount = currentCount;
    transferBuffer.createTransferBuffer(device, difference * sizeof(int) );
    uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, transferBuffer.buffer(), false);
    SDL_memcpy(positionMapped, tempIdUpload.data(), difference * sizeof(int));
    SDL_UnmapGPUTransferBuffer(device, transferBuffer.buffer());

    SDL_GPUTransferBufferLocation possrc = {};
    SDL_GPUBufferRegion           posdst = {};

    possrc.transfer_buffer = transferBuffer.buffer();
    possrc.offset = 0;
    posdst.buffer = textureIDBuffer.buffer();
    posdst.offset = offset;
    posdst.size   = difference * sizeof(int);
    SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);
  }
  lastCount = currentCount;
}


void Texture::getAtlasData(SDL_GPUCopyPass* copypass)
{
  if (loaded) return;

  BW::File file = {"../src/data/json/texture/atlas.json", "rb"};

  char buf[16384];
  rapidjson::FileReadStream is(file.fp, buf, sizeof(buf));

  rapidjson::Document entityFile;
  entityFile.ParseStream(is);

  atlasDimension.push_back({entityFile["dimension"]["w"].GetFloat(), entityFile["dimension"]["h"].GetFloat() });
 
  const auto& darray = entityFile["image"];

  for ( const auto& b : darray.GetArray())
  {
    spDimension.push_back({ b["x"].GetFloat() / atlasDimension[0].w, b["y"].GetFloat() / atlasDimension[0].h,  b["w"].GetFloat() / atlasDimension[0].w, b["h"].GetFloat() / atlasDimension[0].h});
  }
  for ( const auto& a : spDimension)
  {
    std::cout << a.x <<spDimension[0].x<< '\n';
  }

  layout.createBuffer(device, sizeof(float) * 4 * spDimension.size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);

  layoutTrans.createTransferBuffer(device, sizeof(float) * 4 * spDimension.size());

  void* mapped = (void*)SDL_MapGPUTransferBuffer(device, layoutTrans.buffer(), false);
  memcpy(mapped, spDimension.data(), spDimension.size() * sizeof(float) * 4);
  SDL_UnmapGPUTransferBuffer(device, layoutTrans.buffer());

  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = layoutTrans.buffer();
  possrc.offset = 0;
  posdst.buffer = layout.buffer();
  posdst.offset = 0;
  posdst.size   = sizeof(float) * 4 * spDimension.size();

  SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);
}

void Texture::bindSamplers(SDL_GPURenderPass* pass, uint32_t slot)
{
  SDL_BindGPUFragmentSamplers(pass, slot, &binding , 1);
}
void Texture::bindLayout(SDL_GPURenderPass* pass, uint32_t slot)
{
  if (type->size() <= 0 ) return;
  SDL_GPUBuffer* bufs[] = { layout.buffer(), textureIDBuffer.buffer() };

  SDL_BindGPUVertexStorageBuffers( pass, slot, bufs, 2);
}