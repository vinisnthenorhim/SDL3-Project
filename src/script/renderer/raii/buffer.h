#pragma once 

#include <iostream>
#include <SDL3/SDL.h>
#include <vector>
#include "check.h"
#include <functional>

class Buffer
{
  public:
    void createBuffer(SDL_GPUDevice* device, uint32_t size, SDL_GPUBufferUsageFlags usage)
    {
      if (buff != nullptr) return;
      if (size == 0) return;
      this->device = device;
      this->usage = usage;

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
      if (oldBuffer)
      {
          SDL_ReleaseGPUBuffer(device, oldBuffer);
          oldBuffer = nullptr;
      }
    }
    void updateBufferSize(SDL_GPUCopyPass* copypass, Uint32 size, bool copy)
    {
      if (copy)
      {
        updateBufferSizeCopy(copypass, size);
      }
      else
      {
        updateBufferSizeNoCopy(size);
      }
    }
    void updateBufferSizeCopy(SDL_GPUCopyPass* copypass, Uint32 size)
    {
      if (size <= currentSize) return;

      SDL_GPUBufferCreateInfo newBufferInfo{};
      newBufferInfo.size = size;
      newBufferInfo.usage = usage;

      SDL_GPUBuffer* newBuffer = SDL_CreateGPUBuffer(device, &newBufferInfo);
      CHECK(newBuffer);
      if (!newBuffer) return;

      if (buff && currentSize > 0)
      {

        SDL_GPUBufferLocation src = {};
        src.buffer = buff;        
        src.offset = 0;                   

        SDL_GPUBufferLocation dst = {};
        dst.buffer = newBuffer;  
        dst.offset = 0;                         

        SDL_CopyGPUBufferToBuffer(copypass, &src, &dst, currentSize, false);
      }
      oldBuffer = buff;
      buff = newBuffer;
      currentSize = size;
    }
    void updateBufferSizeNoCopy(Uint32 size)
    {
      if (size <= currentSize) return;

      SDL_ReleaseGPUBuffer(device, buff);

      SDL_GPUBufferCreateInfo info{};
      info.size = size;
      info.usage = usage;

      buff = SDL_CreateGPUBuffer(device, &info);

      currentSize = size;
    }
    void releaseOldBuffer()
    {
      if (oldBuffer != nullptr)
      {
        SDL_ReleaseGPUBuffer(device, oldBuffer);
        oldBuffer = nullptr;
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
    SDL_GPUDevice*         device    = nullptr;
    SDL_GPUBuffer*         buff      = nullptr;
    SDL_GPUBuffer*         oldBuffer = nullptr;
    SDL_GPUTransferBuffer* transfer  = nullptr;
    SDL_GPUBufferUsageFlags usage{};
    Uint32 currentSize = 0;
};

// class StorageBuffer
// {
//   public:
//     void createBuffer(SDL_GPUDevice* device, uint32_t size, SDL_GPUBufferUsageFlags usage)
//     {
//       this->device = device;
//       SDL_GPUBufferCreateInfo info {};
//       info.usage = usage;
//       info.size = size;
//       buff = SDL_CreateGPUBuffer(device, &info);
//       CHECK(buff);
//     }
//     ~StorageBuffer()
//     {
//       deleteBuff();
//     }
//     template <typename T>
//     void startSetData(SDL_GPUCopyPass* copy, std::vector<T>& data )
//     {
//       SDL_GPUTransferBufferCreateInfo ti = {};
//       ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
//       ti.size  = sizeof(data) * data.size();
//       transfer = SDL_CreateGPUTransferBuffer(device, &ti);

//       uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, transfer, false);
//       SDL_memcpy(mapped, data.data(), sizeof(T) * data.size());
//       SDL_UnmapGPUTransferBuffer(device, transfer);

//       SDL_GPUTransferBufferLocation src = {};
//       src.transfer_buffer = transfer;
//       src.offset = 0;

//       SDL_GPUBufferRegion dst = {};
//       dst.buffer = buff;
//       dst.offset = 0;
//       dst.size   = sizeof(T) * data.size();

//       SDL_UploadToGPUBuffer(copy, &src, &dst, false);

//     }
//     void endSetData()
//     {
//       SDL_ReleaseGPUTransferBuffer(device, transfer);  
//     }
//     SDL_GPUBuffer* buffer() {return buff;}
//     void deleteBuff()
//     {
//       if(buff)
//       {
//         SDL_ReleaseGPUBuffer(device, buff);
//         buff = nullptr;
//       }
//     }
//   private:
//     SDL_GPUDevice*         device   = nullptr;
//     SDL_GPUBuffer*         buff     = nullptr;
//     SDL_GPUTransferBuffer* transfer = nullptr;
//     SDL_GPUCopyPass* copy = nullptr;


// };

class TransferBuffer
{
  private:
    SDL_GPUDevice* device = nullptr;
    SDL_GPUTransferBuffer* transfer = nullptr;
    uint8_t* mappedPtr = nullptr;
    Uint32 count = 0;
    Uint32 size  = 0;
  public:
    TransferBuffer() = default;                                    
    TransferBuffer(const TransferBuffer&) = delete;
    TransferBuffer& operator=(const TransferBuffer&) = delete;

    void createTransferBuffer(SDL_GPUDevice* device, Uint32 size)
    {
      if (size == 0) return;
      this->device = device;
      this->size = size;
      if(transfer != nullptr) 
      {
        release();
      }
      SDL_GPUTransferBufferCreateInfo ti = {};
      ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      ti.size  = size;
      ;
      transfer = SDL_CreateGPUTransferBuffer(device, &ti);
      CHECK(transfer);
    }
    void mapped(bool cycle)
    {
      count = 0;
      mappedPtr = (uint8_t*)SDL_MapGPUTransferBuffer(device, transfer, cycle);

    }void mappedData(float data)
    {
      if (!mappedPtr || (count + 1) * sizeof(float) > size) return;
      SDL_memcpy(mappedPtr + count * 4, &data, sizeof(float));
      count++;
    }
    void unmapped(SDL_GPUCopyPass* copy)
    {
      SDL_UnmapGPUTransferBuffer(device, transfer);
      mappedPtr = nullptr;
    }
    SDL_GPUTransferBuffer* buffer() {return transfer;}
    ~TransferBuffer()
    {
      release();
    }
    void release()
    {
      if(!transfer) return;
      SDL_ReleaseGPUTransferBuffer(device, transfer);
      transfer = nullptr;
    }
};