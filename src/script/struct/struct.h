#pragma once
#include <iostream>
#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include <SDL3/SDL.h>

typedef std::unordered_map<int64_t, std::unordered_map<int64_t, std::vector<int>>> Grid;
typedef const std::unordered_map<std::string, int>* typeIDsData;

struct ObjectDimension
{
  float width;
  float height;
};

struct ObjectDataRO
{
  const float* secondPerFrame;
  const uint64_t* playerId                     ;

  const std::vector<int>   *type          ;
  const std::vector<int>   *imageId       ;
  const std::vector<float> *transparency  ;
  const std::vector<bool>  *exist         ;

  const std::vector<ObjectDimension> *dimension;

  const std::vector<float>* speedX       ;
  const std::vector<float>* speedY       ;  

  const std::vector<float>* positionX    ;
  const std::vector<float>* positionY    ;
  const std::vector<float>* positionZ    ; 

  const std::vector<float>* scalingX     ;
  const std::vector<float>* scalingY     ;
  const std::vector<float>* scalingZ     ;
  
  const std::vector<float>* rotationX    ;
  const std::vector<float>* rotationY    ;
  const std::vector<float>* rotationZ    ;

  const std::vector<float>* velocityX    ;
  const std::vector<float>* velocityY    ;
  const std::vector<float>* velocityZ    ;

  const std::vector<float>* scaleX       ;
  const std::vector<float>* scaleY       ;
  const std::vector<float>* scaleZ       ;

  const std::vector<float>* rotateX      ;
  const std::vector<float>* rotateY      ;
  const std::vector<float>* rotateZ      ;
};


struct ObjectDataRW
{
  float* secondPerFrame;
  uint64_t* playerId;

  std::vector<int>   *type          ;
  std::vector<int>   *imageId       ;
  std::vector<float> *transparency  ;
  std::vector<bool>  *exist         ;

  std::vector<ObjectDimension> *dimension;

  std::vector<float>* speedX       ;
  std::vector<float>* speedY       ;  

  std::vector<float>* positionX    ;
  std::vector<float>* positionY    ;
  std::vector<float>* positionZ    ; 

  std::vector<float>* scalingX       ;
  std::vector<float>* scalingY       ;
  std::vector<float>* scalingZ       ;

  std::vector<float>* rotationX    ;
  std::vector<float>* rotationY    ;
  std::vector<float>* rotationZ    ;

  std::vector<float>* velocityX    ;
  std::vector<float>* velocityY    ;
  std::vector<float>* velocityZ    ;

  std::vector<float>* scaleX    ;
  std::vector<float>* scaleY    ;
  std::vector<float>* scaleZ    ;

  std::vector<float>* rotateX    ;
  std::vector<float>* rotateY    ;
  std::vector<float>* rotateZ    ;
};
struct DirtyData
{
  std::vector<bool>*  dirtyImage    ;

  std::vector<bool>*  dirtyPosition;
  std::vector<bool>*  dirtyScaling ;
  std::vector<bool>*  dirtyRotation;

  std::vector<bool>*  dirtyVelocity;
  std::vector<bool>*  dirtyScale   ;
  std::vector<bool>*  dirtyRotate  ;
};
struct CameraData
{
  const bool*  follow;
  const float*   deltaTime;
  bool*    dirty;
  const std::array<float, 4>* updateDataX ;
  const std::array<float, 4>* updateDataY ;
  const std::array<float, 4>* updateDataZ ;
  const std::array<float, 4>* positionX ;
  const std::array<float, 4>* positionY ;
  const std::array<float, 4>* positionZ ;

  const glm::mat4* rotateTranpose;
};
struct CameraDataRW
{
  bool*  follow;
  float*   deltaTime;
  bool*    dirty;
  std::array<float, 4>* updateDataX ;
  std::array<float, 4>* updateDataY ;
  std::array<float, 4>* updateDataZ ;
  std::array<float, 4>* positionX ;
  std::array<float, 4>* positionY ;
  std::array<float, 4>* positionZ ;

  glm::mat4* rotateTranpose;
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



