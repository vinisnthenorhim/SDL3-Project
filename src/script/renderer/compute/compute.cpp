#include <iostream>

#include "compute.h"

void Compute::createCompute(SDL_GPUDevice* device)
{
  this->device = device;
  ComputeShader comp(device, "model.comp");

  SDL_GPUComputePipelineCreateInfo pipelineInfo{};
  pipelineInfo.code = comp.compInfo().code;
  pipelineInfo.code_size = comp.compInfo().code_size;
  pipelineInfo.entrypoint = comp.compInfo().entrypoint;
  pipelineInfo.format = comp.compInfo().format;
  pipelineInfo.num_samplers = 0;
  pipelineInfo.num_readonly_storage_textures = 0;
  pipelineInfo.num_readonly_storage_buffers = 1;
  pipelineInfo.num_readwrite_storage_textures = 0;
  pipelineInfo.num_readwrite_storage_buffers = 2;
  pipelineInfo.num_uniform_buffers = 1;
  pipelineInfo.threadcount_x = 64;
  pipelineInfo.threadcount_y = 1;
  pipelineInfo.threadcount_z = 1;

  compPipeline.createComputePipeline(device, pipelineInfo);
}
void Compute::resourceLoad(RenderVelocity& renderVelocity)
{

  this->deltaTime = renderVelocity.secondFrame;
  this->velocityX = renderVelocity.velocityX;
  this->velocityY = renderVelocity.velocityY;
  this->velocityZ = renderVelocity.velocityZ;

}


void Compute::updateStorageBuffer()
{
 
  Uint32 veloXSize = sizeof(float) * velocityX->size();
  Uint32 veloYSize = sizeof(float) * velocityY->size();
  Uint32 veloZSize = sizeof(float) * velocityZ->size();
  Uint32 totalVeloSize = veloXSize + veloYSize + veloZSize;

  int currentEntityCount = velocityX->size();
  if (currentEntityCount == 0) return;                       // CHANGED: guard zero entities

  bool resized = false;                                      // CHANGED: new flag

  if (currentEntityCount > entityCount || modelBuffer.buffer() == nullptr || velocityBuffer.buffer() == nullptr) 
  {
    modelBuffer.createBuffer(device, sizeof(glm::mat4) * (*velocityX).size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE | SDL_GPU_BUFFERUSAGE_GRAPHICS_STORAGE_READ);

    velocityBuffer.createBuffer(device, totalVeloSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ );
    positionBuffer.createBuffer(device, sizeof(glm::vec4) * currentEntityCount, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);   // CHANGED: vec3 -> vec4

    resized = true;                                          // CHANGED

    std::cout << "[SDLGPU]"  << " Storage Buffer resized" << '\n'; 
  }
  entityCount = currentEntityCount;

    if (resized && positionBuffer.buffer() && entityCount !=0 )   // CHANGED: only zero when the buffers are new
    {

      std::vector<glm::vec4> zeros(entityCount, glm::vec4(0.0f));
      const Uint32 bytes = sizeof(glm::vec4) * entityCount;

      SDL_GPUTransferBufferCreateInfo ti = {};
      ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
      ti.size  = bytes;
      SDL_GPUTransferBuffer* tb = SDL_CreateGPUTransferBuffer(device, &ti);

      void* mapped = SDL_MapGPUTransferBuffer(device, tb, false);
      SDL_memcpy(mapped, zeros.data(), bytes);
      SDL_UnmapGPUTransferBuffer(device, tb);

      SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
      SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);

      SDL_GPUTransferBufferLocation src = {};
      src.transfer_buffer = tb;

      SDL_GPUBufferRegion dst = {};
      dst.buffer = positionBuffer.buffer();
      dst.size   = bytes;

      SDL_UploadToGPUBuffer(copy, &src, &dst, false);
      SDL_EndGPUCopyPass(copy);
      SDL_SubmitGPUCommandBuffer(cmd);

      SDL_ReleaseGPUTransferBuffer(device, tb);   // safe after submit
    std::cout << "[SDLGPU]"  << " Storage Buffer resized" << '\n'; 

    }
  SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);

  SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);

  SDL_GPUTransferBufferCreateInfo ti = {};
  ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
  ti.size  = totalVeloSize;

  transfer = SDL_CreateGPUTransferBuffer(device, &ti);

  uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, transfer, false);
  SDL_memcpy(mapped                        , (*velocityX).data(), veloXSize);
  SDL_memcpy(mapped + veloXSize            , (*velocityY).data(), veloYSize);
  SDL_memcpy(mapped + veloXSize + veloYSize, (*velocityZ).data(), veloZSize);

  SDL_UnmapGPUTransferBuffer(device, transfer);

  SDL_GPUTransferBufferLocation src = {};
  src.transfer_buffer = transfer;
  src.offset = 0;

  SDL_GPUBufferRegion dst = {};
  dst.buffer = velocityBuffer.buffer();
  dst.offset = 0;
  dst.size   = totalVeloSize;

  SDL_UploadToGPUBuffer(copy, &src, &dst, false);

  // std::cout << "[SDLGPU]"  << " Storage Buffer updated " << '\n'; 
  
  SDL_EndGPUCopyPass(copy);

  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);

  SDL_WaitForGPUFences(device, true, &fence, 1);

  SDL_ReleaseGPUTransferBuffer(device, transfer);

  SDL_ReleaseGPUFence(device, fence);
}

struct UniformData
{
  float deltaTime;
  Uint32 entityCount;
};

void Compute::bindComp(SDL_GPUCommandBuffer* cmd)
{
  if (!compPipeline.pipeline() || entityCount == 0) return;
  Uint32 groupCount = (entityCount + 64 - 1) / 64;
  UniformData uniformData = {*deltaTime, entityCount};

  SDL_GPUStorageBufferReadWriteBinding rw[2] = {};
  rw[0].buffer = modelBuffer.buffer();
  rw[0].cycle = false;

  rw[1].buffer = positionBuffer.buffer();
  rw[1].cycle = false;

  SDL_GPUComputePass* pass = SDL_BeginGPUComputePass(cmd, nullptr, 0, rw, 2 );

  SDL_BindGPUComputePipeline(pass, compPipeline.pipeline());

  SDL_GPUBuffer* ro[] = {
      velocityBuffer.buffer()
  };

  SDL_BindGPUComputeStorageBuffers( pass, 0, ro, 1 );

  SDL_PushGPUComputeUniformData(cmd, 0, &uniformData, sizeof(uniformData));

  SDL_DispatchGPUCompute(pass, groupCount, 1, 1);

  SDL_EndGPUComputePass(pass);
}


void Compute::calculate()
{ 
  // std::cout << (*velocityX)[0] << '\n';
  if ((*velocityX).size() == 0) return;

  SDL_GPUTransferBufferCreateInfo ti = {};
  ti.usage = SDL_GPU_TRANSFERBUFFERUSAGE_DOWNLOAD;
  ti.size  = sizeof(glm::vec4) * (*velocityX).size();

  SDL_GPUTransferBuffer* download = SDL_CreateGPUTransferBuffer(device, &ti);

  SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
  SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);

  SDL_GPUBufferRegion src = {};
  src.buffer = positionBuffer.buffer();
  src.offset = 0;
  src.size   = sizeof(glm::vec4) * (*velocityX).size();

  SDL_GPUTransferBufferLocation dst = {};
  dst.transfer_buffer = download;
  dst.offset = 0;
  SDL_DownloadFromGPUBuffer(copy, &src, &dst);   // the source comes first here

  SDL_EndGPUCopyPass(copy);
  

  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);

  SDL_WaitForGPUFences(device, true, &fence, 1);


  SDL_ReleaseGPUFence(device, fence);

    glm::vec4* mats = (glm::vec4*)SDL_MapGPUTransferBuffer(device, download, false);
      if (mats) {
          for (Uint32 i = 0; i < (*velocityX).size(); i++) {
              const glm::vec4& m = mats[i];
              SDL_Log("model[%u]  translation = %.2f %.2f %.2f", i, m[0], m[1], m[2]);
          }
      }
  SDL_UnmapGPUTransferBuffer(device, download);

  SDL_ReleaseGPUTransferBuffer(device, download);
  
}