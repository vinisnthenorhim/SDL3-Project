#pragma once

#include <renderer/raii/_raii.h>
#include <game/renderObject/enum/struct.h>

class ObjectTransform
{
  private:
    ComputePipeline computePipeline;

    TransferBuffer positionTransfer;
    TransferBuffer scalingTransfer ;
    TransferBuffer rotationTransfer;

    TransferBuffer veloTrans;
    TransferBuffer scalTrans;
    TransferBuffer rotaTrans;

    Buffer positionBuffer;
    Buffer scalingBuffer;
    Buffer rotationBuffer;

    Buffer velocityBuffer;
    Buffer scaleBuffer;
    Buffer rotateBuffer;

    Buffer modelBuffer;

    const std::vector<float>* positionX    ;   const std::vector<float>* scalingX    ;  const std::vector<float>* rotationX    ;
    const std::vector<float>* positionY    ;   const std::vector<float>* scalingY    ;  const std::vector<float>* rotationY    ;
    const std::vector<float>* positionZ    ;   const std::vector<float>* scalingZ    ;  const std::vector<float>* rotationZ    ;

    std::vector<bool>  *dirtyVelocity;  std::vector<bool>  *dirtyScale   ;  std::vector<bool>  *dirtyRotate  ;
    std::vector<float> *velocityX    ;  std::vector<float> *scaleX       ;  std::vector<float> *rotateX      ;
    std::vector<float> *velocityY    ;  std::vector<float> *scaleY       ;  std::vector<float> *rotateY      ;
    std::vector<float> *velocityZ    ;  std::vector<float> *scaleZ       ;  std::vector<float> *rotateZ      ;

    const std::vector<float> *speedX       ;  const std::vector<float> *speedY       ;

    const std::vector<bool> *visible       ;  float *deltaTime                 ;

    std::vector<glm::vec3> position;
    std::vector<glm::vec3> scaling;
    std::vector<glm::vec3> rotation;

    Uint32 entityCount = 0;

    SDL_GPUDevice* device = nullptr;

  public:
    void createObjectTransform(SDL_GPUDevice* device);
    void loadData( ObjectData objectData, ObjectTransformData objectTransform);
    void updateDataSize( SDL_GPUCopyPass* copypass);
    void updateTransform( SDL_GPUCopyPass* copypass);
    void bind( SDL_GPUCommandBuffer* cmd);
    void release();
    SDL_GPUBuffer* getModel() { return modelBuffer.buffer(); }

};