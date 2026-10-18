#pragma once
#include <game/renderObject/enum/struct.h>
#include <renderer/raii/_raii.h>

class CameraHandle
{
  private:
    Uint32* follow;
    Uint32* entityId;
    float* deltaTime;
    bool* reset;
    bool* dirty;
    bool created = false;

    std::array<float, 4>* camPositionX = nullptr;
    std::array<float, 4>* camPositionY = nullptr;
    std::array<float, 4>* camPositionZ = nullptr;

    std::array<float, 4>* updateDataX = nullptr;
    std::array<float, 4>* updateDataY = nullptr;
    std::array<float, 4>* updateDataZ = nullptr;

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
    void followEntity( SDL_GPUCopyPass* copypass );
    void releaseOld();
    void bind(SDL_GPUCommandBuffer* cmd,  SDL_GPUBuffer* position);

    SDL_GPUBuffer* getCamera() { return cameraMatrice.buffer(); }
};