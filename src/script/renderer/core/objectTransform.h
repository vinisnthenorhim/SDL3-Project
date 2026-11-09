#pragma once

#include <renderer/raii/_raii.h>
#include <struct/struct.h>

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
    const std::vector<float> *velocityX    ;   const std::vector<float> *scaleX      ;  const std::vector<float> *rotateX      ;
    const std::vector<float> *velocityY    ;   const std::vector<float> *scaleY      ;  const std::vector<float> *rotateY      ;
    const std::vector<float> *velocityZ    ;   const std::vector<float> *scaleZ      ;  const std::vector<float> *rotateZ      ;

    const std::vector<float> *speedX       ;  const std::vector<float> *speedY       ;

    const std::vector<bool> *exist         ;  const float *secondPerFrame            ;

    std::vector<bool>*  dirtyPosition;
    std::vector<bool>*  dirtyScaling ;
    std::vector<bool>*  dirtyRotation;

    std::vector<bool>*  dirtyVelocity;
    std::vector<bool>*  dirtyScale   ;
    std::vector<bool>*  dirtyRotate  ;

    Uint32 entityCount = 0;

    SDL_GPUDevice* device = nullptr;

  public:
    void createObjectTransform(SDL_GPUDevice* device);
    void loadData( ObjectDataRO objectDataRO, DirtyData dirtyData);
    void updateDataSize( SDL_GPUCopyPass* copypass );
    void updateTransform( SDL_GPUCopyPass* copypass );
    void bind( SDL_GPUCommandBuffer* cmd);
    void bindG(SDL_GPURenderPass* pass, uint32_t slot);
    void release();
    SDL_GPUBuffer* getPos() { return positionBuffer.buffer(); }

};