#pragma once

#include <vector>
#include <GLM/glm.hpp>
#include <struct/struct.h>
#include <logic/updaterate/rate.h>

class Movement
{
  private:
    float* deltaTime;
    std::vector<float> *positionX    ;
    std::vector<float> *positionY    ;
    std::vector<float> *positionZ    ;
    std::vector<bool>  *dirtyVelocity;
    std::vector<float> *velocityX    ;
    std::vector<float> *velocityY    ;
    std::vector<float> *velocityZ    ;
    std::vector<float> *speedX       ;
    std::vector<float> *speedY       ;

    uint32_t* playerId;
    bool moveit = false;
    int itterate = 8;
    int step = 0;
    TimeRate rate;
  public:
    Movement()
    : rate(30)
    {}

    void loadData(MovementStructData data);
    void move( bool nn = false );
};