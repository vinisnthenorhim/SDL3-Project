#pragma once

#include <SDL3/SDL.h>
#include <game/renderObject/enum/struct.h>

class Camera
{
  private:
    float positionX = 0;
    float positionY = 0;
    float positionZ = 0;

    float velocityX = 0  ;
    float velocityY = 0  ;
    float velocityZ = 0  ;

    const float speedXYZ = 800.0f;

    uint8_t lastAxisX = 1;
    uint8_t lastAxisY = 1;


    float lastFrame = 0;
    float delta = 0;

    Uint32 followPlayer = 0u;
    Uint32 playerId = 0;
    bool dirty = false;
  public:
    Camera();
    void cameraPosition();
    void deltaTime();
    Position getCameraPosition();
};