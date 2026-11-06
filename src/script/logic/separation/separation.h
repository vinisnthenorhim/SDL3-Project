#pragma once

#include <vector>
#include <array>
#include <GLM/glm.hpp>
#include <struct/struct.h>

class ObjectSeparation
{
  private:
    float* deltaTime;
    std::vector<ObjectDimension> *dimension;

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

    std::vector<glm::vec2> nn;
    std::vector<int> animatingCount;
    std::vector<bool> isAnimating;

    std::array<float, 4> *camPositionX ;
    std::array<float, 4> *camPositionY ;
    std::array<float, 4> *camPositionZ ;

    const int gridsize = 100;
    const int updateSize = 20;
    const int checksize = 1;
    const int step = 10;
    const Grid* cell;

    uint64_t* playerId;
    uint32_t* follow;

    uint32_t lastSize = 0;

  public:

    void loadData( ObjectDataRW objectDataRW, DirtyData dirtyData, CameraDataRW cameraDataRW, const Grid* cell);
    void checkCollide();
    void doAnimate();

};