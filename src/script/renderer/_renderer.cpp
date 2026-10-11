#include "_renderer.h"

#include "check.h"

Renderer::Renderer(SDL_Window* window)
{
  this->window = window;
  objectDevice.createDevice(window, SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL);

  device = objectDevice.gpuDevice();


  log_properties("device", SDL_GetGPUDeviceProperties(device));
  SDL_Log(SDL_GetGPUDeviceDriver(device));

  projection.createProjection(window, device);

  objectRender.createObjectRender(window, device);

  compute.createCompute(device);
}


Renderer::~Renderer()
{
  SDL_WaitForGPUIdle(device);

}


void Renderer::screenUpdate(const SDL_Event* event)
{
  if (event->type == SDL_EVENT_WINDOW_RESIZED)
  {
    projection.updateProjection();
  }
}


void Renderer::resourceDataLoad(const RenderStoreResource& resource, RenderObjectPosition objectPosition, RenderVelocity objectVelocity)
{
  compute.resourceLoad(objectVelocity);
  objectRender.resourceLoad(resource);
}


void Renderer::transfer()
{
  SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);

  SDL_GPUCopyPass* copy = SDL_BeginGPUCopyPass(cmd);
  objectRender.uploadData(copy);

  SDL_EndGPUCopyPass(copy);
  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);

  SDL_WaitForGPUFences(device, true, &fence, 1);

  objectRender.endUpdate();

  SDL_ReleaseGPUFence(device, fence);
}


bool Renderer::copy()
{
  compute.updateStorageBuffer();
  compute.calculate();
  cmdBuffer = SDL_AcquireGPUCommandBuffer(device); 
  if(!cmdBuffer) return false;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);
  if(!swap) return false;

  return true;
}

bool Renderer::render()
{

  compute.bindComp(cmdBuffer);
  projection.uploadProjection(cmdBuffer);

  SDL_GPUColorTargetInfo target {};

  target.texture = swap;
  target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;
  SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);

  objectRender.bind(cmdBuffer, pass, compute.getModel());

  SDL_EndGPURenderPass(pass);

  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdBuffer);


  SDL_WaitForGPUFences(device, true, &fence, 1);


  SDL_ReleaseGPUFence(device, fence);

  return true;
}
