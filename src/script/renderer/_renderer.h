#ifndef RENDERER_H
#define RENDERER_H


#include <SDL3/SDL.h>
#include <game/renderObject/enum/struct.h>
#include <renderer/raii/_raii.h>
#include <renderer/compute/compute.h>
#include "ObjectRender.h"
#include "projection.h"

class Renderer
{
    public:
        Renderer(SDL_Window* window);
        ~Renderer();
        void resourceDataLoad(const RenderStoreResource& resource, RenderObjectPosition objectPosition, RenderVelocity objectVelocity);

        void screenUpdate(const SDL_Event* event);
        void transfer();
        bool copy();
        bool render();

    private:
      SDL_Window*           window    = nullptr;
      SDL_GPUDevice*        device    = nullptr;
      SDL_GPUCommandBuffer* cmdBuffer = nullptr;
      SDL_GPUTexture*       swap      = nullptr;

      Device          objectDevice;
      GraphicPipeline pipeline;
      Compute         compute;
      Buffer          vertexBuffer;
      ObjectRender    objectRender;
      Projection      projection;


};

#endif