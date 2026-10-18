#pragma once

#include <renderer/raii/_raii.h>
#include <game/renderObject/enum/struct.h>

class ObjectRender
{   
  public:
    void createObjectRender(SDL_Window* window, SDL_GPUDevice* device);
    void resourceLoad(ObjectData objectData);
    void uploadData(SDL_GPUCopyPass* copy);
    void endUpdate();
    void bind( SDL_GPURenderPass* pass, SDL_GPUBuffer* model, SDL_GPUBuffer* camera);

  private:
    SDL_GPUDevice* device = nullptr;
    bool upload = false;

    GraphicPipeline gPipeline;
    
    Buffer vertBuff;
    Buffer indexBuff;

    const std::vector<bool>* visible       ;
   
  
};