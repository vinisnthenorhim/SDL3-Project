#include "camera.h"
#include <cmath>
#include <iostream>

Camera::Camera()
{
  lastFrame = SDL_GetTicksNS() / 1000000000;
}

void Camera::cameraPosition()
{
  if (followPlayer) return;

  int8_t axisX = 0;
  int8_t axisY = 0;

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool arrowUp    = keyboardState[SDL_SCANCODE_UP   ];
  bool arrowDown  = keyboardState[SDL_SCANCODE_DOWN ];
  bool arrowLeft  = keyboardState[SDL_SCANCODE_LEFT ];
  bool arrowRight = keyboardState[SDL_SCANCODE_RIGHT];


  if      (arrowLeft && !arrowRight) lastAxisX = -1;
  else if (!arrowLeft && arrowRight) lastAxisX =  1;

  if      (arrowDown && !arrowUp) lastAxisY = -1;
  else if (!arrowDown && arrowUp) lastAxisY =  1;

  if (arrowLeft && arrowRight)    axisX = -lastAxisX;
  else if (arrowLeft)             axisX = -1;
  else if (arrowRight)            axisX =  1;
  if (!arrowLeft && !arrowRight)  axisX =  0;

  if (arrowDown && arrowUp)       axisY = -lastAxisY;
  else if (arrowDown)             axisY = -1;
  else if (arrowUp)               axisY =  1;
  if (!arrowDown && !arrowUp)     axisY =  0;

  const float length = std::sqrt(float( axisX * axisX + axisY * axisY));

  const float newVelocityX = length > 0 ? axisX / length : 0.0f;
  const float newVelocityY = length > 0 ? axisY / length : 0.0f;

  if ( velocityX != newVelocityX || velocityY != newVelocityY || velocityX != 0 || velocityY != 0)
  {
    velocityX = newVelocityX * speedXYZ;
    velocityY = newVelocityY * speedXYZ;
    dirty = true;
  }
  else
  {
    velocityX = 0;
    velocityY = 0;
    dirty = false;
  }


}

void Camera::deltaTime()
{
  float currentFrame = SDL_GetTicksNS();

  delta = ( currentFrame - lastFrame) / 1000000000;
  lastFrame = currentFrame;
}


Position Camera::getCameraPosition()
{
  return
  {
    &followPlayer,
    &playerId,
    &delta,
    &dirty,
    &velocityX,
    &velocityY,
    &velocityZ,
    &positionX,
    &positionY,
    &positionZ
  };
}