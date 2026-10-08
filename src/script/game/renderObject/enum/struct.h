#ifndef STRUCT_ENTITY_H
#define STRUCT_ENTITY_H
#include "enum.h"
#include <glm/glm.hpp>

struct RenderStoreResource
{
  const std::vector<Type>* type;
  const std::vector<bool>* visible;

  const std::vector<float>* speedX       ;
  const std::vector<float>* speedY       ;
    
  const std::vector<float>* positionX    ;
  const std::vector<float>* positionY    ;
  const std::vector<float>* positionZ    ;
  
  const std::vector<float>* scaleX       ;
  const std::vector<float>* scaleY       ;
  const std::vector<float>* scaleZ       ;

  const std::vector<float>* rotationX    ;
  const std::vector<float>* rotationY    ;
  const std::vector<float>* rotationZ    ;
};

struct RenderObjectPosition
{
  std::vector<float>* positionX    ;
  std::vector<float>* positionY    ;
  std::vector<float>* positionZ    ;
  
  std::vector<float>* scaleX       ;
  std::vector<float>* scaleY       ;
  std::vector<float>* scaleZ       ;

  std::vector<float>* rotationX    ;
  std::vector<float>* rotationY    ;
  std::vector<float>* rotationZ    ;
};
struct RenderVelocity
{
  float* secondFrame;
  std::vector<bool>*  dirty         ;

  std::vector<float>* velocityX    ;
  std::vector<float>* velocityY    ;
  std::vector<float>* velocityZ    ;
};
#endif