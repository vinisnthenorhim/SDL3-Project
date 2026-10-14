#pragma once
#include "enum.h"
#include <glm/glm.hpp>

struct ObjectData
{
  const std::vector<bool>* visible;

  const std::vector<float>* speedX       ;
  const std::vector<float>* speedY       ;  

  std::vector<float>* positionX    ;
  std::vector<float>* positionY    ;
  std::vector<float>* positionZ    ; 

  std::vector<float>* scalingX       ;
  std::vector<float>* scalingY       ;
  std::vector<float>* scalingZ       ;
  
  std::vector<float>* rotationX    ;
  std::vector<float>* rotationY    ;
  std::vector<float>* rotationZ    ;
};


struct ObjectTransformData
{
  float* secondPerFrame;

  std::vector<bool>*  dirtyVelocity;
  std::vector<float>* velocityX    ;
  std::vector<float>* velocityY    ;
  std::vector<float>* velocityZ    ;

  std::vector<bool>*  dirtyScale;
  std::vector<float>* scaleX    ;
  std::vector<float>* scaleY    ;
  std::vector<float>* scaleZ    ;

  std::vector<bool>*  dirtyRotate;
  std::vector<float>* rotateX    ;
  std::vector<float>* rotateY    ;
  std::vector<float>* rotateZ    ;
};
struct Position
{
  float positionX    ;
  float positionY    ;
  // float positionZ    ;
  
  // float scalingX     ;
  // float scalingY     ;
  // float scalingZ     ;

  // float rotationX    ;
  // float rotationY    ;
  // float rotationZ    ;

  Position(float x, float y)
  :positionX(x), positionY(y)
  {}
};