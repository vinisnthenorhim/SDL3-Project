#include "camera.h"
#include <cmath>
#include <iostream>

Camera::Camera(SDL_Window* window)
{
  lastFrame = SDL_GetTicksNS() / 1000000000;

  positionX[1] = 1;
  positionY[1] = 1;
  positionZ[1] = 1;
  int x, y, w, h, cw, ch;


  SDL_GetWindowPosition(window, &x, &y);
  SDL_GetWindowSize(window, &cw, &ch);
  SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
  const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);

  x = x - mode->w/2      + cw/2;
  y = -1 * (y - mode->h/2 ) - ch/2;

  lastWinPosX = x;
  lastWinPosY = y;
}

void Camera::cameraHandle(SDL_Window* window)
{
  dirty = cameraPosition() || cameraZoom() || cameraRotate() || movingWindow(window);
}

bool Camera::cameraPosition()
{
  if (followPlayer) return false;

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

  if ( updateDataX[0] != newVelocityX || updateDataY[0] != newVelocityY || updateDataX[0] != 0 || updateDataX[0] != 0)
  {
    updateDataX[0] = newVelocityX * speedXYZ;
    updateDataY[0] = newVelocityY * speedXYZ;
    return true;
  }
  else
  {
    updateDataX[0] = 0;
    updateDataY[0] = 0;
    return false;
  }
}
bool Camera::cameraZoom()
{  
  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool zoomIn    = keyboardState[SDL_SCANCODE_L ];
  bool zoomOut   = keyboardState[SDL_SCANCODE_K ];


  int8_t axis = 0;

  if (zoomIn && zoomOut) axis = 0;
  else if (zoomIn && !zoomOut) axis = -1;
  else if (!zoomIn && zoomOut) axis = 1 ;
  else if (!zoomIn && !zoomOut) axis = 0;

  float zoom = axis * zoomSpeed;

  if ((updateDataX[1] != 0 || updateDataY[1] != 0 || zoom != 0) && (positionX[1] + zoom *delta >= 0.2f || positionY[1] + zoom * delta >= 0.2f))
  {
    updateDataX[1] = zoom;
    updateDataY[1] = zoom;
    positionX[1] += zoom * delta;
    positionY[1] += zoom * delta;
    return true;
  }
  else if( updateDataX[1] != 0 || updateDataY[1] != 0)
  {
    updateDataX[1] = 0;
    updateDataY[1] = 0;
    return true;
  }
  else
  {
    updateDataX[1] = 0;
    updateDataY[1] = 0;
    return false;
  }

}
bool Camera::cameraRotate()
{
  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool clockwise         = keyboardState[SDL_SCANCODE_M ];
  bool counterclockwise  = keyboardState[SDL_SCANCODE_N ];

  int8_t axis = 0;

  if (clockwise && counterclockwise) axis = 0;
  else if (clockwise && !counterclockwise) axis = 1;
  else if (!clockwise && counterclockwise) axis = -1;
  else axis = 0;

  float rotate = axis * rotateSpeed;

  if(axis != 0)
  {
    updateDataX[2] = 0;
    updateDataY[2] = 0;
    updateDataZ[2] = rotate;
    return true;
  }
  else if (updateDataX[2] != 0 || updateDataY[2] != 0 || updateDataZ[2] != 0)
  {
    updateDataX[2] = 0;
    updateDataY[2] = 0;
    updateDataZ[2] = 0;
    return true;
  }
  else
  {
    return false;
  }


  return false;

}
bool Camera::movingWindow(SDL_Window* window)
{
  int x, y, w, h, cw, ch;

  SDL_GetWindowPosition(window, &x, &y);
  SDL_GetWindowSize(window, &cw, &ch);
  SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
  const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);

  x = x - mode->w/2      + cw/2;
  y = -1 * (y - mode->h/2 ) - ch/2;
  
  if (lastWinPosX != x ||lastWinPosY != y)
  {
    updateDataX[3] = x;
    updateDataY[3] = y;
    lastWinPosX = x;
    lastWinPosY = y;
    return true;
  }
  else
  {
    lastWinPosX = x;
    lastWinPosY = y;
    return false;
  }
}
void Camera::setSettings(const SDL_Event* event)
{
  if (event->key.key == SDLK_U && event->key.down && !event->key.repeat)
  {
    if (followPlayer == 0u) followPlayer = 1u;
    else followPlayer = 0u;
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
    &updateDataX,
    &updateDataY,
    &updateDataZ,
    &positionX,
    &positionY,
    &positionZ
  };
}