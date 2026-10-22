#pragma once

#include <renderer/raii/_raii.h>
#include <struct/struct.h>

class ObjectRender
{   
  public:
    void createObjectRender(SDL_Window* window, SDL_GPUDevice* device);
    void resourceLoad(ObjectData objectData);
    void uploadData(SDL_GPUCopyPass* copy);
    void endUpdate();
    void startBind(SDL_GPURenderPass* pass);
    void endBind(SDL_GPURenderPass* pass);

  private:
    SDL_GPUDevice* device = nullptr;
    bool upload = false;

    GraphicPipeline gPipeline;
    
    Buffer vertBuff;
    Buffer indexBuff;

    const std::vector<bool>* visible       ;
   
  
};