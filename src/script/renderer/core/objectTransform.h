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

    std::vector<float>* positionX    ;   std::vector<float>* scalingX    ;  std::vector<float>* rotationX    ;
    std::vector<float>* positionY    ;   std::vector<float>* scalingY    ;  std::vector<float>* rotationY    ;
    std::vector<float>* positionZ    ;   std::vector<float>* scalingZ    ;  std::vector<float>* rotationZ    ;

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
    void saveData(SDL_GPUCommandBuffer* cmd);
    void release();
    SDL_GPUBuffer* getModel() { return modelBuffer.buffer(); }
    SDL_GPUBuffer* getPos() { return positionBuffer.buffer(); }

};