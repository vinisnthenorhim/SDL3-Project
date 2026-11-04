#pragma once
#include <struct/struct.h>
#include <renderer/raii/_raii.h>

class CameraHandle
{
  private:
    const uint32_t* follow;
    const uint64_t* entityId;
    const float* deltaTime;
    const bool* reset;
    const bool* dirty;
    bool created = false;

    const std::array<float, 4>* camPositionX = nullptr;
    const std::array<float, 4>* camPositionY = nullptr;
    const std::array<float, 4>* camPositionZ = nullptr;

    const std::array<float, 4>* updateDataX = nullptr;
    const std::array<float, 4>* updateDataY = nullptr;
    const std::array<float, 4>* updateDataZ = nullptr;

    const std::vector<float>* positionX    ;
    const std::vector<float>* positionY    ;
    const std::vector<float>* positionZ    ;
    
    ComputePipeline cameraPipeline;

    Buffer cameraVelocity;
    Buffer cameraPosition;
    Buffer cameraMatrice;

    TransferBuffer positionTransfer;
    TransferBuffer posUpdateTransfer;

    SDL_GPUDevice* device = nullptr;
  public:
    void createCameraHandle(SDL_GPUDevice* device);
    void loadData( ObjectDataRO objectData, CameraData cameraData );

    void createCamera( SDL_GPUCopyPass* copypass );
    void updateCameraData( SDL_GPUCopyPass* copypass );
    void followEntity( SDL_GPUCopyPass* copypass );
    void releaseOld();
    void bind(SDL_GPUCommandBuffer* cmd,  SDL_GPUBuffer* position);
    void bindG(SDL_GPURenderPass* pass, uint32_t slot);

    SDL_GPUBuffer* getCamera() { return cameraMatrice.buffer(); }
};