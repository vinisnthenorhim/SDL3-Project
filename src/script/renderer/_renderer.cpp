#include "_renderer.h"

#include "check.h"

Renderer::Renderer(SDL_Window* window)
{
  this->window = window;
  device = SDL_CreateGPUDevice(SDL_GPU_SHADERFORMAT_SPIRV | SDL_GPU_SHADERFORMAT_DXIL | SDL_GPU_SHADERFORMAT_MSL , true, nullptr);
  CHECK(device);
  CHECK(SDL_ClaimWindowForGPUDevice(device, window));


  log_properties("device", SDL_GetGPUDeviceProperties(device));
  SDL_Log(SDL_GetGPUDeviceDriver(device));

  projection.createProjection(window, device);

  objectRender.createObjectRender(window, device, cmdBuffer);
}


Renderer::~Renderer()
{
  SDL_ReleaseWindowFromGPUDevice(device, window);
  SDL_DestroyGPUDevice(device);
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
  objectRender.resourceLoad(resource);
}

bool Renderer::render()
{
  cmdBuffer = SDL_AcquireGPUCommandBuffer(device); 
  if(!cmdBuffer) return false;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);



  if(swap)
  {
    projection.uploadProjection(cmdBuffer);

    SDL_GPUColorTargetInfo target {};

    target.texture = swap;
    target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
    target.load_op = SDL_GPU_LOADOP_CLEAR;
    target.store_op = SDL_GPU_STOREOP_STORE;
    SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);

    objectRender.bind(cmdBuffer, pass);

    SDL_EndGPURenderPass(pass);
  }

  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdBuffer);


  SDL_WaitForGPUFences(device, true, &fence, 1);


  SDL_ReleaseGPUFence(device, fence);

  return true;
}
