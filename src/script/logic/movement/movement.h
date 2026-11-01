#pragma once

#include <vector>
#include <array>
#include <GLM/glm.hpp>
#include <struct/struct.h>
#include <logic/updaterate/rate.h>
#include <RVO.h>

class Movement
{
  private:
    float* deltaTime;
    std::vector<ObjectDimension> *dimension;
    std::vector<float> *positionX    ;
    std::vector<float> *positionY    ;
    std::vector<float> *positionZ    ;

    std::vector<float> *scalingX     ;
    std::vector<float> *scalingY     ;
    std::vector<float> *scalingZ     ;

    std::vector<float> *rotationX    ;
    std::vector<float> *rotationY    ;
    std::vector<float> *rotationZ    ;

    std::vector<bool>  *dirtyVelocity;
    std::vector<float> *velocityX    ;
    std::vector<float> *velocityY    ;
    std::vector<float> *velocityZ    ;

    std::vector<float> *speedX       ;
    std::vector<float> *speedY       ;

    std::vector<glm::vec2> nn;
    std::vector<int> animatingCount;
    std::vector<bool> isAnimating;

    std::array<float, 4> *camPositionX ;
    std::array<float, 4> *camPositionY ;
    std::array<float, 4> *camPositionZ ;

    const int gridsize = 100;
    const int updateSize = 20;
    const int checksize = 1;
    const int step = 20;
    Grid cell;

    uint32_t* playerId;
    uint32_t* follow;

    uint32_t lastSize = 0;

  public:


    void loadData( ObjectData objectData, MovementStructData data , Position cameraPosition);
    void move( float dx, float dy, float dz );
    void updateCell();
    void doAnimate();

    glm::vec3 getCenter();

};