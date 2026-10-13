#include "_renderer.h"

#include "check.h"

Renderer::Renderer(SDL_Window* window)
{
  this->window = window;
  objectDevice.createDevice(window, SDL_GPU_SHADERFORMAT_SPIRV );

  device = objectDevice.gpuDevice();


  log_properties("device", SDL_GetGPUDeviceProperties(device));
  SDL_Log(SDL_GetGPUDeviceDriver(device));

  projection.createProjection(window, device);

  objectTransform.createObjectTransform(device);

  objectRender.createObjectRender(window, device);



  // compute.createCompute(device);
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


void Renderer::dataLoad(ObjectData objectData, ObjectTransformData objectTransformData)
{
  objectTransform.loadData(objectData, objectTransformData);
  // compute.resourceLoad(objectData, objectTransformData);
  objectRender.resourceLoad(objectData);
}


// void Renderer::transfer()
// {
//   SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);

//   copy = SDL_BeginGPUCopyPass(cmd);

//   SDL_EndGPUCopyPass(copy);
//   if(!copy) SDL_Log("no copy pass");
//   SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);

//   SDL_WaitForGPUFences(device, true, &fence, 1);


//   SDL_ReleaseGPUFence(device, fence);
// }


// bool Renderer::copypass()
// {
//   compute.updateStorageBuffer();
//   cmdBuffer = SDL_AcquireGPUCommandBuffer(device); 
//   if(!cmdBuffer) return false;
//   SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);
//   if(!swap) return false;

//   return true;
// }

// bool Renderer::r()
// {

//   compute.bindComp(cmdBuffer);
//   projection.uploadProjection(cmdBuffer);

//   SDL_GPUColorTargetInfo target {};

//   target.texture = swap;
//   target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
//   target.load_op = SDL_GPU_LOADOP_CLEAR;
//   target.store_op = SDL_GPU_STOREOP_STORE;
//   SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);

//   objectRender.bind( pass, compute.getModel());

//   SDL_EndGPURenderPass(pass);

//   SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmdBuffer);


//   SDL_WaitForGPUFences(device, true, &fence, 1);


//   SDL_ReleaseGPUFence(device, fence);

//   return true;
// }


void Renderer::render()
{
  // SDL_Log("1");
  cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
  if(!cmdBuffer) return;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);


  copypass = SDL_BeginGPUCopyPass(cmdBuffer);
  // SDL_Log("2");

  objectTransform.updateDataSize(copypass);
  // SDL_Log("3");
  objectTransform.updateTransform(copypass);

  // SDL_Log("4");
  objectRender.uploadData(copypass);

  SDL_EndGPUCopyPass(copypass);

  objectTransform.bind(cmdBuffer);
  // SDL_Log("5");


  SDL_GPUColorTargetInfo target {};

  target.texture = swap;
  target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;

  // SDL_Log("6");

  renderpass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);

  // SDL_Log("7");
  objectRender.bind( renderpass, objectTransform.getModel() );

  SDL_EndGPURenderPass(renderpass);

  // SDL_Log("8");
  projection.uploadProjection(cmdBuffer);

  // SDL_Log("9");
  SDL_SubmitGPUCommandBuffer(cmdBuffer);

  objectTransform.release();
  objectRender.endUpdate();

}