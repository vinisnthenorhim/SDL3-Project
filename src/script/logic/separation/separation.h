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

  std::vector<float> *transparency  ;
  std::vector<bool>*  dirtyImage    ;

    std::vector<glm::vec2> nn;
    std::vector<int> animatingCount;
    std::vector<bool> isAnimating;

    const int gridsize = 100;
    const int checksize = 1;
    const int step = 13;
    const Grid* cell;

    uint64_t* playerId;
    uint32_t* follow;

    uint32_t lastSize = 0;

  public:

    void loadData( ObjectDataRW objectDataRW, DirtyData dirtyData, const Grid* cell);
    void updateSize();
    void checkCollide();
    void doAnimate();

};