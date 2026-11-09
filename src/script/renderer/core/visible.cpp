#include "visible.h"


void Visible::loadData( ObjectDataRO objectDataRO, DirtyData dirtyData )
{
  this->dirtyImage    = dirtyData.dirtyImage      ;
  this->exist         = objectDataRO.exist        ;
  this->imageId       = objectDataRO.imageId      ;
  this->transparency  = objectDataRO.transparency ;
}

void Visible::update(SDL_GPUDevice* device, SDL_GPUCopyPass* copypass)
{
  if ( exist->size() <= 0 ) return;

  bool dirty = false;
  for ( auto a : *dirtyImage )
  {
    if ( a == true )
    {
      dirty = true;
      for (auto b : *dirtyImage)
      {
        b = false;
      }
      break;
    }
  }
  if ( !dirty ) return;
  for ( auto a : *exist)
  {
    if ( a == true )
    {
      existUint8.emplace_back(1);
    }
    else
    {
      existUint8.emplace_back(0);
    }
  }

  if (!existBuffer        .buffer()) existBuffer       .createBuffer( device, sizeof(int)   * existUint8.size()   , SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ );
  else (existBuffer       .updateBufferSize( copypass, sizeof(int)   * existUint8.size()      , false ));
  if (!imageIdBuffer      .buffer()) imageIdBuffer     .createBuffer( device, sizeof(int)   * imageId->size()     , SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ );
  else (imageIdBuffer     .updateBufferSize( copypass, sizeof(int)   * imageId->size()     , false ));
  if (!transparencyBuffer .buffer()) transparencyBuffer.createBuffer( device, sizeof(float) * transparency->size(), SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ );
  else (transparencyBuffer.updateBufferSize( copypass, sizeof(float) * transparency->size(), false ));

  existTransfer       .createTransferBuffer( device, sizeof(int)   * existUint8.size()       );
  imageIdTransfer     .createTransferBuffer( device, sizeof(int)   * imageId->size()     );
  transparencyTransfer.createTransferBuffer( device, sizeof(float) * transparency->size());

  if (existTransfer.buffer())
  {
    void* mapped = SDL_MapGPUTransferBuffer( device, existTransfer.buffer(), false);
    SDL_memcpy(mapped, existUint8.data(), sizeof(int) * existUint8.size());
    SDL_UnmapGPUTransferBuffer( device, existTransfer.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = existTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = existBuffer.buffer();
    dst.size   = sizeof(int) * existUint8.size();
    dst.offset = 0         ; 
    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }
  if (imageIdTransfer.buffer())
  {
    void* mapped = SDL_MapGPUTransferBuffer(device, imageIdTransfer.buffer(), false);
    SDL_memcpy( mapped, imageId->data(), sizeof(int) * imageId->size() );
    SDL_UnmapGPUTransferBuffer(device, imageIdTransfer.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = imageIdTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = imageIdBuffer.buffer();
    dst.size   = sizeof(int) * imageId->size();
    dst.offset = 0         ; 
    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
  }
  if (transparencyTransfer.buffer())
  {
    void* mapped = SDL_MapGPUTransferBuffer(device, transparencyTransfer.buffer(), false);
    SDL_memcpy( mapped, transparency->data(), sizeof(float) * transparency->size() );
    SDL_UnmapGPUTransferBuffer(device, transparencyTransfer.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = transparencyTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = transparencyBuffer.buffer();
    dst.size   = sizeof(float) * transparency->size();
    dst.offset = 0         ; 
    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
  }
}

void Visible::release()
{
    existBuffer       .releaseOldBuffer();
    imageIdBuffer     .releaseOldBuffer();
    transparencyBuffer.releaseOldBuffer();
}

void Visible::bindG( SDL_GPURenderPass* pass, uint32_t vertslot, uint32_t fragslot )
{
  if (exist->empty()) return;
  SDL_GPUBuffer* vertbufs[] = 
  {
    imageIdBuffer.buffer(),
    existBuffer.buffer()
  };
  SDL_GPUBuffer* fragbufs[] = 
  {
    transparencyBuffer.buffer()
  };

  SDL_BindGPUVertexStorageBuffers( pass, vertslot, vertbufs, 2);
  SDL_BindGPUFragmentStorageBuffers( pass, fragslot, fragbufs, 1);
}