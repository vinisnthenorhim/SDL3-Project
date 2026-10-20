#pragma once

#include <SDL3/SDL.h>
#include "shader.h"
#include "check.h"
class GraphicPipeline
{
  public:
  void createGraphicPipeline(SDL_GPUDevice* device, SDL_GPUGraphicsPipelineCreateInfo info )
  {
    this->device = device;
    pipeline = SDL_CreateGPUGraphicsPipeline(device, &info);
    CHECK(pipeline);
    if (!pipeline) {
        return;
    }
  }
  void bind(SDL_GPURenderPass* pass)
  {
    SDL_BindGPUGraphicsPipeline(pass, pipeline);
  }
  ~GraphicPipeline()
  {
    if(pipeline)
    {
      SDL_ReleaseGPUGraphicsPipeline(device, pipeline);
    }
  }
  private:
    SDL_GPUDevice* device = nullptr;
    SDL_GPUGraphicsPipeline* pipeline = nullptr;
};

class ComputePipeline
{
  public:
    void createComputePipeline(SDL_GPUDevice* device,  SDL_GPUComputePipelineCreateInfo &pipelineInfo)
    {
      this->device = device;
      compPipeline = SDL_CreateGPUComputePipeline(device, &pipelineInfo); 
      CHECK(compPipeline);
    }
    ~ComputePipeline()
    {
      destroy();
    }
    void destroy()
    {
      if (!compPipeline) return;
      SDL_ReleaseGPUComputePipeline(device, compPipeline);
      compPipeline = nullptr;
    }
    SDL_GPUComputePipeline* pipeline() {return compPipeline;}

  private:
    SDL_GPUComputePipeline* compPipeline = nullptr;
    SDL_GPUDevice* device = nullptr;
};