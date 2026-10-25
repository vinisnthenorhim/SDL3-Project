#include "camera.h"
#include <cmath>
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
Camera::Camera(SDL_Window* window)
{
  this->window = window;
  BW::File fp = {"../src/data/json/settings/camera.json", "rb"};

  char buf[16384];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document camera_path;
  camera_path.ParseStream(is);
  
  camera_enable = camera_path["camera"].GetBool();
  if (!camera_enable) return;
  up    = SDL_GetScancodeFromName(camera_path["camera_movement"]["up"]   .GetString());
  down  = SDL_GetScancodeFromName(camera_path["camera_movement"]["down"] .GetString());
  left  = SDL_GetScancodeFromName(camera_path["camera_movement"]["left"] .GetString());
  right = SDL_GetScancodeFromName(camera_path["camera_movement"]["right"].GetString());

  x_axis.counterclockwise = SDL_GetScancodeFromName(camera_path["camera_rotation"]["x_axis"]["counterclockwise"].GetString());
  x_axis.clockwise        = SDL_GetScancodeFromName(camera_path["camera_rotation"]["x_axis"]["clockwise"]       .GetString());
  y_axis.counterclockwise = SDL_GetScancodeFromName(camera_path["camera_rotation"]["y_axis"]["counterclockwise"].GetString());
  y_axis.clockwise        = SDL_GetScancodeFromName(camera_path["camera_rotation"]["y_axis"]["clockwise"]       .GetString());
  z_axis.counterclockwise = SDL_GetScancodeFromName(camera_path["camera_rotation"]["z_axis"]["counterclockwise"].GetString());
  z_axis.clockwise        = SDL_GetScancodeFromName(camera_path["camera_rotation"]["z_axis"]["clockwise"]       .GetString());

  zoomIn  = SDL_GetScancodeFromName(camera_path["camera_zoom"]["in"] .GetString());
  zoomOut = SDL_GetScancodeFromName(camera_path["camera_zoom"]["out"].GetString());

  lastFrame = SDL_GetTicksNS() / 1000000000;

  positionX[1] = 1;
  positionY[1] = 1;
  positionZ[1] = 1;
  updateDataX[1] = 1;
  updateDataY[1] = 1;
  updateDataZ[1] = 1;
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

void Camera::cameraHandle()
{
  if (!camera_enable) return;
  bool p = cameraPosition();
  bool z = cameraZoom();
  bool r = cameraRotate();
  bool w = movingWindow();
  dirty = p || z || r || w;

}

bool Camera::cameraPosition()
{

  if (followPlayer) return false;

  int8_t axisX = 0;
  int8_t axisY = 0;

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool arrowUp    = keyboardState[up];
  bool arrowDown  = keyboardState[down];
  bool arrowLeft  = keyboardState[left];
  bool arrowRight = keyboardState[right];


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

  const float zoomSpeedRatio = 1 / positionX[1];

  if ( updateDataX[0] != newVelocityX || updateDataY[0] != newVelocityY || updateDataZ[0] != 0 || updateDataZ[0] != 0)
  {
    updateDataX[0] = zoomSpeedRatio * newVelocityX * speedXYZ;
    updateDataY[0] = zoomSpeedRatio * newVelocityY * speedXYZ;
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

  if (followPlayer) return false;

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool zoomIn    = keyboardState[this->zoomIn];
  bool zoomOut   = keyboardState[this->zoomOut];

  float zoomSpeed = 1;

  if      (zoomIn && zoomOut  ) zoomSpeed = 1.0; 
  else if (zoomIn && !zoomOut ) zoomSpeed = 1.02;
  else if (!zoomIn && zoomOut ) zoomSpeed = 0.98;
  else if (!zoomIn && !zoomOut) zoomSpeed = 1.0;

  if ((zoomSpeed != 1) && ( positionX[1] * zoomSpeed >= 0.2 || positionY[1] * zoomSpeed >= 0.2 || positionZ[1] * zoomSpeed >= 0.2) && ( positionX[1] * zoomSpeed <= 10.0 || positionY[1] * zoomSpeed <= 10.0 || positionZ[1] * zoomSpeed <= 10.0))
  {
    updateDataX[1] = zoomSpeed;
    updateDataY[1] = zoomSpeed;
    updateDataZ[1] = zoomSpeed;
    positionX[1]  *= zoomSpeed ;
    positionY[1]  *= zoomSpeed ;
    positionZ[1]  *= zoomSpeed ;
    return true;
  }
  else if( updateDataX[1] != 1 || updateDataY[1] != 1)
  {
    updateDataX[1] = 1 ;
    updateDataY[1] = 1 ;
    updateDataZ[1] = 1 ;

    return true;
  }
  else
  {
    updateDataX[1] = 1 ;
    updateDataY[1] = 1 ;
    updateDataZ[1] = 1 ;
    return false;
  }

}
bool Camera::cameraRotate()
{

  if (followPlayer) return false;

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool x_axis_clockwise         = keyboardState[x_axis.clockwise];
  bool x_axis_counterclockwise  = keyboardState[x_axis.counterclockwise];
  bool y_axis_clockwise         = keyboardState[y_axis.clockwise];
  bool y_axis_counterclockwise  = keyboardState[y_axis.counterclockwise];
  bool z_axis_clockwise         = keyboardState[z_axis.clockwise];
  bool z_axis_counterclockwise  = keyboardState[z_axis.counterclockwise];
  int8_t x_direction = 0;
  int8_t y_direction = 0;
  int8_t z_direction = 0;

  if (x_axis_clockwise && x_axis_counterclockwise)       x_direction = 0;
  else if (x_axis_clockwise && !x_axis_counterclockwise) x_direction = 1;
  else if (!x_axis_clockwise && x_axis_counterclockwise) x_direction = -1;
  else x_direction = 0;

  if (y_axis_clockwise && y_axis_counterclockwise)       y_direction = 0;
  else if (y_axis_clockwise && !y_axis_counterclockwise) y_direction = 1;
  else if (!y_axis_clockwise && y_axis_counterclockwise) y_direction = -1;
  else y_direction = 0;

  if (z_axis_clockwise && z_axis_counterclockwise)       z_direction = 0;
  else if (z_axis_clockwise && !z_axis_counterclockwise) z_direction = 1;
  else if (!z_axis_clockwise && z_axis_counterclockwise) z_direction = -1;
  else z_direction = 0;

  float rotate_x = x_direction * rotateSpeed;
  float rotate_y = y_direction * rotateSpeed;
  float rotate_z = z_direction * rotateSpeed;

  if(x_direction != 0 || y_direction != 0 ||z_direction != 0)
  {
    updateDataX[2] = rotate_x;
    updateDataY[2] = rotate_y;
    updateDataZ[2] = rotate_z;
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
}
bool Camera::movingWindow()
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
  if (!camera_enable) return;
  if (event->key.key == SDLK_U && event->type == SDL_EVENT_KEY_DOWN && !event->key.repeat)
  {
    if (followPlayer == 0u) followPlayer = 1u;
    else followPlayer = 0u;
    positionX[1] = 1;
    positionY[1] = 1;
    positionZ[1] = 1;
  }
}


void Camera::deltaTime()
{
  if (!camera_enable) return;

  float currentFrame = SDL_GetTicksNS();

  delta = ( currentFrame - lastFrame) / 1000000000;
  lastFrame = currentFrame;
}


Position Camera::getCameraPosition()
{
  return
  {
    &followPlayer,
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

void Camera::itterate()
{
  deltaTime();
  cameraHandle();
}

void Camera::cameraEvent(const SDL_Event* event)
{
  setSettings(event);
}