#pragma once

#include <glm/glm.hpp>
#include <renderer/raii/_raii.h>
#include <game/renderObject/enum/struct.h>

class Compute
{
  public:
    void createCompute(SDL_GPUDevice* device);

    void resourceLoad(RenderVelocity& renderVelocity);
    void calculate();
    void createComputePipeline();
    void updateStorageBuffer();
    void endSetStorageBuffer();
    void bindComp(SDL_GPUCommandBuffer* cmd);
    SDL_GPUBuffer* getModel() { return modelBuffer.buffer();}

  private:
    SDL_GPUDevice* device = nullptr;
    SDL_GPUTransferBuffer* transfer = nullptr;
    ComputePipeline compPipeline;
    Buffer positionBuffer;
    Buffer modelBuffer;
    Buffer velocityBuffer;

    int entityCount = 0;


    float* deltaTime;

    std::vector<float>* velocityX    ;
    std::vector<float>* velocityY    ;
    std::vector<float>* velocityZ    ;


};