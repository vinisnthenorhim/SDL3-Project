#pragma once

#include <renderer/raii/_raii.h>
#include <game/renderObject/enum/struct.h>

class ObjectRender
{   
  public:
    void createObjectRender(SDL_Window* window, SDL_GPUDevice* device, SDL_GPUCommandBuffer* cmd);
    void resourceLoad(const RenderStoreResource resource);
    void uploadData();
    void endUpdate();
    void bind(SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass* pass);

  private:
    SDL_GPUDevice* device = nullptr;

    GraphicPipeline gPipeline;
    
    Buffer vertBuff;
    Buffer indexBuff;

    const std::vector<Type>* type          ;
    const std::vector<bool>* visible       ;
    
    const std::vector<float>* speedX       ;
    const std::vector<float>* speedY       ;
      
    const std::vector<float>* positionX    ;
    const std::vector<float>* positionY    ;
    const std::vector<float>* positionZ    ;
    
    const std::vector<float>* scaleX       ;
    const std::vector<float>* scaleY       ;
    const std::vector<float>* scaleZ       ;

    const std::vector<float>* rotationX    ;
    const std::vector<float>* rotationY    ;
    const std::vector<float>* rotationZ    ;
  
};