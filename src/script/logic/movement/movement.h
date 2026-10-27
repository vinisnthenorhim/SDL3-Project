#pragma once

#include <vector>
#include <GLM/glm.hpp>
#include <struct/struct.h>
#include <logic/updaterate/rate.h>
#include <RVO.h>

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
    RVO::RVOSimulator sim;
    int lastsize = 0;
    const float stopDist = 60.0f;
  public:
    Movement()
    : rate(30)
    {
      sim.setTimeStep(1.0f / 20.0f);        // match how often you call doStep()
      sim.setAgentDefaults(
          40.0f,   // neighborDist
          8,      // maxNeighbors
          2.0f,   // timeHorizon
          60.0f,   // radius (agents are spheres)
          300.0f);  // maxSpeed
    }

    void loadData(MovementStructData data);
    void move( bool nn = false );
};