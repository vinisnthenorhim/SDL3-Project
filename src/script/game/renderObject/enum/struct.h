#pragma once
#include <iostream>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

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
  Uint32*  follow;
  Uint32*  entityID;
  float*   deltaTime;
  bool*    dirty;
  std::array<float, 4>* updateDataX ;
  std::array<float, 4>* updateDataY ;
  std::array<float, 4>* updateDataZ ;
  std::array<float, 4>* positionX ;
  std::array<float, 4>* positionY ;
  std::array<float, 4>* positionZ ;
};

struct CreateEntityLayout
{
  float posX;
  float posY;
  float posZ;

  float sclX;
  float sclY;
  float sclZ;

  float rotX;
  float rotY;
  float rotZ;
};

struct RotateAxis
{
  SDL_Scancode counterclockwise;
  SDL_Scancode clockwise;
};