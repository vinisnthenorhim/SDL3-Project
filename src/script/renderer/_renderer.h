#ifndef RENDERER_H
#define RENDERER_H


#include <SDL3/SDL.h>
#include <renderer/raii/_raii.h>
#include <renderer/core/ObjectRender.h>
#include <renderer/core/projection.h>
#include <renderer/core/ObjectTransform.h>
#include <renderer/core/camera.h>
#include <texture/loader/texture.h>
#include <renderer/core/visible.h>

class Renderer
{
    public:
        Renderer(SDL_Window* window);
        ~Renderer();
        void loadData( ObjectDataRO objectDataRO, DirtyData dirtyData, CameraData cameraData );

        void screenUpdate(const SDL_Event* event);

        void render();

    private:
      SDL_Window*           window    = nullptr;
      SDL_GPUDevice*        device    = nullptr;
      SDL_GPUCommandBuffer* cmdBuffer = nullptr;
      SDL_GPUTexture*       swap      = nullptr;
      SDL_GPUCopyPass*      copypass  = nullptr;
      SDL_GPURenderPass*    renderpass= nullptr;

      Device          objectDevice;
      ObjectTransform objectTransform;
      ObjectRender    objectRender;
      Projection      projection;
      CameraHandle    camera;
      Texture         texture;
      Visible         visible;

};

#endif