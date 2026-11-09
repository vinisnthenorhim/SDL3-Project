#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <loader/BW.h>
#include "_renderer.h"

#include "check.h"

Renderer::Renderer(SDL_Window* window)
{
  this->window = window;
  objectDevice.createDevice(window, true, SDL_GPU_SHADERFORMAT_SPIRV );

  device = objectDevice.gpuDevice();

  SDL_Log(SDL_GetGPUDeviceDriver(device));
  SDL_ShowWindow(window);
  cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
  if(!cmdBuffer) return;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);
  SDL_GPUColorTargetInfo target {};

  target.texture = swap;
  target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;
  renderpass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);
  
  SDL_EndGPURenderPass(renderpass);
  SDL_SubmitGPUCommandBuffer(cmdBuffer);



  projection.createProjection(window, device);

  objectTransform.createObjectTransform(device);
  camera.createCameraHandle(device);

  objectRender.createObjectRender(window, device);
  texture.createTexture(device);
  texture.createSampler();
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


void Renderer::loadData( ObjectDataRO objectDataRO, DirtyData dirtyData, CameraData cameraData )
{
  texture        .loadData (objectDataRO);
  objectTransform.loadData (objectDataRO, dirtyData);
  objectRender   .laodData (objectDataRO);
  camera         .loadData (objectDataRO, cameraData);
  visible        .loadData (objectDataRO, dirtyData);
}



void Renderer::render()
{
  cmdBuffer = SDL_AcquireGPUCommandBuffer(device);
  if(!cmdBuffer) return;
  SDL_WaitAndAcquireGPUSwapchainTexture(cmdBuffer, window, &swap, nullptr, nullptr);

  // SDL_Log("1\n");
  copypass = SDL_BeginGPUCopyPass(cmdBuffer);
  // SDL_Log("2\n");

  objectTransform.updateDataSize(copypass);
  objectTransform.updateTransform(copypass);
  // SDL_Log("3\n");

  camera.createCamera(copypass);

  camera.updateCameraData(copypass);
  objectRender.uploadData(copypass);
  texture.getAtlasData(copypass);
  texture.loadTexture(copypass);
  visible.update( device, copypass );
  // SDL_Log("4\n");

  SDL_EndGPUCopyPass(copypass);
  texture.setBinding();

  objectTransform.bind(cmdBuffer);
  camera.bind(cmdBuffer, objectTransform.getPos());

  // SDL_Log("5\n");

  SDL_GPUColorTargetInfo target {};

  target.texture = swap;
  target.clear_color = (SDL_FColor) {0.0f, 0.0f, 0.0f, 0.0f};
  target.load_op = SDL_GPU_LOADOP_CLEAR;
  target.store_op = SDL_GPU_STOREOP_STORE;


  renderpass = SDL_BeginGPURenderPass(cmdBuffer, &target, 1, nullptr);
  objectRender.startBind(renderpass);

  objectTransform.bindG(renderpass, 0);
  camera.bindG(renderpass, 1);
  texture.bindLayout(renderpass, 2);
  visible.bindG( renderpass, 3, 0);

  texture.bindSamplers( renderpass, 0 );

  objectRender.endBind(renderpass);

  SDL_EndGPURenderPass(renderpass);

  projection.uploadProjection(cmdBuffer);

  SDL_SubmitGPUCommandBuffer(cmdBuffer);

  objectTransform.release();
  objectRender.endUpdate();
  camera.releaseOld();
  texture.release();
  visible.release();
}
