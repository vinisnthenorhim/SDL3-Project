#include "camera.h"
#include <cmath>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
Camera::Camera(SDL_Window* window)
{
  FILE* fp = fopen("../src/script/data/settings/camera.json", "rb");

  if (!fp) return;

  char buf[16384];
  rapidjson::FileReadStream is(fp, buf, sizeof(buf));

  rapidjson::Document camera_path;
  camera_path.ParseStream(is);
  fclose(fp);
  camera_enable = camera_path["camera"].GetBool();
  if (!camera_enable) return;
  up    = SDL_GetScancodeFromName(camera_path["camera_movement"]["up"].GetString());
  down  = SDL_GetScancodeFromName(camera_path["camera_movement"]["down"].GetString());
  left  = SDL_GetScancodeFromName(camera_path["camera_movement"]["left"].GetString());
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
  if (!camera_enable) return;
  dirty = cameraPosition() || cameraZoom() || cameraRotate() || movingWindow(window);
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
  if (followPlayer) return false;

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  bool zoomIn    = keyboardState[this->zoomIn];
  bool zoomOut   = keyboardState[this->zoomOut];

  int8_t axis = 0;

  if (zoomIn && zoomOut) axis = 0;
  else if (zoomIn && !zoomOut) axis = 1;
  else if (!zoomIn && zoomOut) axis = -1 ;
  else if (!zoomIn && !zoomOut) axis = 0;

  float zoom = axis * zoomSpeed;

  if ((updateDataX[1] != 0 || updateDataY[1] != 0 || zoom != 0) && (positionX[1] + zoom *delta >= 0.2f || positionY[1] + zoom * delta >= 0.2f))
  {
    updateDataX[1] = zoom;
    updateDataY[1] = zoom;
    updateDataZ[1] = zoom;
    positionX[1] += zoom * delta;
    positionY[1] += zoom * delta;
    positionZ[1] += zoom * delta;

    return true;
  }
  else if( updateDataX[1] != 0 || updateDataY[1] != 0)
  {
    updateDataX[1] = 0;
    updateDataY[1] = 0;
    updateDataZ[1] = 0;

    return true;
  }
  else
  {
    updateDataX[1] = 0;
    updateDataY[1] = 0;
    updateDataZ[1] = 0;
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

  if(z_direction != 0)
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
  if (!camera_enable) return;
  if (event->key.key == SDLK_U && event->key.down && !event->key.repeat)
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