#include "_renderer.h"

#include "check.h"

Renderer::Renderer(SDL_Window* window)
{
  this->window = window;
  objectDevice.createDevice(window, SDL_GPU_SHADERFORMAT_SPIRV );

  device = objectDevice.gpuDevice();
  
  SDL_Log(SDL_GetGPUDeviceDriver(device));

  projection.createProjection(window, device);

  objectTransform.createObjectTransform(device);
  camera.createCameraHandle(device);

  objectRender.createObjectRender(window, device);
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


void Renderer::dataLoad(ObjectData objectData, ObjectTransformData objectTransformData, Position cameraPosition)
{
  objectTransform.loadData(objectData, objectTransformData);
  objectRender.resourceLoad(objectData);
  camera.loadData(objectData, cameraPosition);
}



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

  camera.createCamera(copypass);
  // SDL_Log("3.5");

  camera.updateCameraData(copypass);
  // SDL_Log("4");
  objectRender.uploadData(copypass);

  SDL_EndGPUCopyPass(copypass);

  objectTransform.bind(cmdBuffer);
  camera.bind(cmdBuffer, objectTransform.getPos());
  // SDL_Log("5");


  SDL_GPUColorTargetInfo target {};

  target.texture = swap;
  target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;

  // SDL_Log("6");

  renderpass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);

  // SDL_Log("7");
  objectRender.bind( renderpass, objectTransform.getModel(), camera.getCamera() );

  SDL_EndGPURenderPass(renderpass);

  // SDL_Log("8");
  projection.uploadProjection(cmdBuffer);

  // SDL_Log("9");
  SDL_SubmitGPUCommandBuffer(cmdBuffer);

  objectTransform.release();
  objectRender.endUpdate();
  camera.releaseOld();

}