#pragma once 

#include <SDL3/SDL.h>
#include <vector>
#include "check.h"

class Buffer
{
  public:
    void createBuffer(SDL_GPUDevice* device, uint32_t size, SDL_GPUBufferUsageFlags usage)
    {
      this->device = device;
      SDL_GPUBufferCreateInfo info {};
      info.usage = usage;
      info.size = size;
      buff = SDL_CreateGPUBuffer(device, &info);
      CHECK(buff);
    }
    ~Buffer()
    {
      if(buff)
      {
        SDL_ReleaseGPUBuffer(device, buff);
        buff = nullptr;
      }

    }
    template <typename T>
    void startSetData(SDL_GPUCopyPass* copy, std::vector<T>& data )
    {
      SDL_GPUTransferBufferCreateInfo ti = {};
      ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      ti.size  = sizeof(data) * data.size();
      transfer = SDL_CreateGPUTransferBuffer(device, &ti);

      void* mapped = SDL_MapGPUTransferBuffer(device, transfer, false);
      SDL_memcpy(mapped, data.data(), sizeof(T) * data.size());
      SDL_UnmapGPUTransferBuffer(device, transfer);

      SDL_GPUTransferBufferLocation src = {};
      src.transfer_buffer = transfer;
      src.offset = 0;

      SDL_GPUBufferRegion dst = {};
      dst.buffer = buff;
      dst.offset = 0;
      dst.size   = sizeof(T) * data.size();

      SDL_UploadToGPUBuffer(copy, &src, &dst, false);

    }
    void endSetData()
    {
      SDL_ReleaseGPUTransferBuffer(device, transfer);  
    }
    SDL_GPUBuffer* buffer() {return buff;}
  private:
    SDL_GPUDevice*         device   = nullptr;
    SDL_GPUBuffer*         buff     = nullptr;
    SDL_GPUTransferBuffer* transfer = nullptr;
};

class StorageBuffer
{
  public:
    void createBuffer(SDL_GPUDevice* device, uint32_t size, SDL_GPUBufferUsageFlags usage)
    {
      this->device = device;
      SDL_GPUBufferCreateInfo info {};
      info.usage = usage;
      info.size = size;
      buff = SDL_CreateGPUBuffer(device, &info);
      CHECK(buff);
    }
    ~StorageBuffer()
    {
      deleteBuff();
    }
    template <typename T>
    void startSetData(SDL_GPUCopyPass* copy, std::vector<T>& data )
    {
      SDL_GPUTransferBufferCreateInfo ti = {};
      ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      ti.size  = sizeof(data) * data.size();
      transfer = SDL_CreateGPUTransferBuffer(device, &ti);

      uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, transfer, false);
      SDL_memcpy(mapped, data.data(), sizeof(T) * data.size());
      SDL_UnmapGPUTransferBuffer(device, transfer);

      SDL_GPUTransferBufferLocation src = {};
      src.transfer_buffer = transfer;
      src.offset = 0;

      SDL_GPUBufferRegion dst = {};
      dst.buffer = buff;
      dst.offset = 0;
      dst.size   = sizeof(T) * data.size();

      SDL_UploadToGPUBuffer(copy, &src, &dst, false);

    }
    void endSetData()
    {
      SDL_ReleaseGPUTransferBuffer(device, transfer);  
    }
    SDL_GPUBuffer* buffer() {return buff;}
    void deleteBuff()
    {
      if(buff)
      {
        SDL_ReleaseGPUBuffer(device, buff);
        buff = nullptr;
      }
    }
  private:
    SDL_GPUDevice*         device   = nullptr;
    SDL_GPUBuffer*         buff     = nullptr;
    SDL_GPUTransferBuffer* transfer = nullptr;
    SDL_GPUCopyPass* copy = nullptr;


};