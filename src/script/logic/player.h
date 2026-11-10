#pragma once

#include <struct/struct.h>
#include <functional>
enum class ControlType
{
  KEYBOARD,
  MOUSE
};

class Player
{
  private:
    ControlType playerControl = ControlType::MOUSE;
    uint64_t* playerId;

    int8_t lastAxisX = 0;
    int8_t lastAxisY = 0;

    float targetX = 0;
    float targetY = 0;

    bool init = false;
    bool wasDown = false;

    float zoom = 0.6;
    glm::vec3 posOffset {0};

    float *secondPerFrame;

    std::vector<bool>  *dirtyImage    ;
    std::vector<int>   *imageId       ;
    std::vector<float> *transparency  ;
    std::vector<bool>  *exist         ;

    std::vector<bool>  *dirtyPosition;
    std::vector<float> *positionX    ;
    std::vector<float> *positionY    ;
    std::vector<float> *positionZ    ;

    std::vector<bool>  *dirtyScaling ;
    std::vector<float> *scalingX     ;
    std::vector<float> *scalingY     ;
    std::vector<float> *scalingZ     ;

    std::vector<bool>  *dirtyRotation ;
    std::vector<float> *rotationX    ;
    std::vector<float> *rotationY    ;
    std::vector<float> *rotationZ    ;
    
    std::vector<float> *speedX       ;
    std::vector<float> *speedY       ;    

    std::array<float, 4> *camPositionX ;
    std::array<float, 4> *camPositionY ;
    std::array<float, 4> *camPositionZ ;

    glm::mat4* rotateTranpose;

    bool* follow;
    bool* camDirty;
    void keyboardControl();
    void mouseControl();
    void animate();
    void camHandle();

    std::function<void(float, float, float, float, uint64_t)> move;

    SDL_Window* window = nullptr;
  public:
    void movement();
    void loadData( ObjectDataRW objectDataRW, DirtyData dirtyData, SDL_Window* window, CameraDataRW cameraDataRW, std::function<void(float, float, float, float, uint64_t)> func);
};