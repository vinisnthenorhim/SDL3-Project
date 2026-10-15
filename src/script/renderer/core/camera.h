#pragma once
#include <game/renderObject/enum/struct.h>
#include <renderer/raii/_raii.h>

class CameraHandle
{
  private:
    Uint32* follow;
    Uint32* entityId;
    float* deltaTime;
    bool* dirty;
    bool created = false;

    float* camPositionX;
    float* camPositionY;
    float* camPositionZ;

    float* velocityX   ;
    float* velocityY   ;
    float* velocityZ   ;

    std::vector<float>* positionX    ;
    std::vector<float>* positionY    ;
    std::vector<float>* positionZ    ;
    
    ComputePipeline cameraPipeline;

    Buffer cameraVelocity;
    Buffer cameraPosition;
    Buffer cameraMatrice;

    TransferBuffer positionTransfer;
    TransferBuffer posUpdateTransfer;

    SDL_GPUDevice* device = nullptr;
  public:
    void createCameraHandle(SDL_GPUDevice* device);
    void loadData(ObjectData objectData, Position cameraPosition);

    void createCamera( SDL_GPUCopyPass* copypass );
    void updateCameraData( SDL_GPUCopyPass* copypass );
    void releaseOld();
    void bind(SDL_GPUCommandBuffer* cmd,  SDL_GPUBuffer* position);

    SDL_GPUBuffer* getCamera() { return cameraMatrice.buffer(); }
};