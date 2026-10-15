#pragma once

#include <SDL3/SDL.h>
#include <game/renderObject/enum/struct.h>

class Camera
{
  private:
    std::array<float, 4> positionX {0};
    std::array<float, 4> positionY {0};
    std::array<float, 4> positionZ {0};

    std::array<float, 4> updateDataX {0}  ;
    std::array<float, 4> updateDataY {0}  ;
    std::array<float, 4> updateDataZ {0}  ;

    const float speedXYZ = 800.0f;
    const float zoomSpeed = 0.5f;
    const float rotateSpeed = 20.0f;

    uint8_t lastAxisX = 1;
    uint8_t lastAxisY = 1;


    float lastFrame = 0;
    float delta = 0;

    Uint32 followPlayer = 0u;
    Uint32 playerId = 0;
    bool dirty = false;
  public:
    Camera();
    void cameraHandle();
    bool cameraPosition();
    bool cameraZoom();
    bool cameraRotate();
    void deltaTime();
    Position getCameraPosition();
    void movingWindow(SDL_Window* window,const SDL_Event* event);
};