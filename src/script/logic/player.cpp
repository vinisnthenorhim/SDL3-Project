#include "player.h"

void Player::movement()
{
  if (exist->size() <= *playerId) return;
  if ( !init )
  {
    targetX = (*positionX)[*playerId];
    targetY = (*positionY)[*playerId];

    (*camPositionX)[0] = (*positionX)[*playerId];
    (*camPositionY)[0] = (*positionY)[*playerId];
    (*camPositionZ)[0] = (*positionZ)[*playerId];

    (*camPositionX)[1] = zoom;
    (*camPositionY)[1] = zoom;
    (*camPositionZ)[1] = zoom;

    (*camPositionX)[2] = 0;
    (*camPositionY)[2] = 0;
    (*camPositionZ)[2] = 0;

    *camDirty = true;
    init = true;
  }

  switch (playerControl)
  {
    case ControlType::KEYBOARD :
      keyboardControl();
      break;
    case ControlType::MOUSE :
      mouseControl();
      animate();
      break;
    default:
      keyboardControl();
      break;
  }
  camHandle();
}

void Player::keyboardControl()
{

  const bool* keyboardState = SDL_GetKeyboardState(nullptr);

  int8_t axisX = 0;
  int8_t axisY = 0;

  bool keyA = keyboardState[SDL_SCANCODE_A];
  bool keyD = keyboardState[SDL_SCANCODE_D];
  bool keyW = keyboardState[SDL_SCANCODE_W];
  bool keyS = keyboardState[SDL_SCANCODE_S];

  if      (keyA && !keyD) lastAxisX = -1;
  else if (!keyA && keyD) lastAxisX =  1;

  if      (keyS && !keyW) lastAxisY = -1;
  else if (!keyS && keyW) lastAxisY =  1;

  if (keyA && keyD)   axisX = -lastAxisX;
  else if (keyA)      axisX = -1;
  else if (keyD)      axisX =  1;
  if (!keyA && !keyD) axisX =  0;

  if (keyS && keyW)   axisY = -lastAxisY;
  else if (keyS)      axisY = -1;
  else if (keyW)      axisY =  1;
  if (!keyS && !keyW) axisY =  0;

  const float length = std::sqrt(float( axisX * axisX + axisY * axisY));

  const float directionX = length > 0 ? axisX / length : 0.0f;
  const float directionY = length > 0 ? axisY / length : 0.0f;

  const float dirX = directionX * (*speedX)[*playerId];
  const float dirY = directionY * (*speedY)[*playerId];

  if ( axisX != 0 || axisY != 0)
  {

    (*positionX)[*playerId] += dirX * *secondPerFrame;
    (*positionY)[*playerId] += dirY * *secondPerFrame;
 
    (*dirtyPosition)[*playerId] = true;

  }
}
void Player::mouseControl()
{
  float xm, ym;
  int x, y, cw, ch;

  SDL_GetWindowPosition(window, &x, &y);
  SDL_GetWindowSize(window, &cw, &ch);
  SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
  const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);

  x = x - mode->w/2      + cw/2;
  y = -1 * (y - mode->h/2 ) - ch/2;

  SDL_MouseButtonFlags buttons = SDL_GetMouseState( &xm, &ym );
  bool isDown = (buttons & SDL_BUTTON_LMASK) != 0;
  float offX = ( (xm - cw / 2.f) / zoom)  ;
  float offY = (-(ym - ch / 2.f) / zoom)  ;     
  if ( isDown && !wasDown)
  {
    glm::vec4 tempVec( offX, offY, 0.0f, 0.0f );
    auto temp = *rotateTranpose * tempVec;

    targetX = (*positionX)[*playerId] + temp.x + posOffset.x;
    targetY = (*positionY)[*playerId] + temp.y + posOffset.y;
  }
  wasDown = isDown;
}
void Player::animate()
{
  float compX = targetX - (*positionX)[*playerId];
  float compY = targetY - (*positionY)[*playerId];
  if (fabsf(compX) > 10 || fabsf(compY) > 10)
  {
    const float length = std::sqrt(float( compX * compX + compY * compY));

    const float directionX = length > 0 ? compX / length : 0.0f;
    const float directionY = length > 0 ? compY / length : 0.0f;


    const float dirX = directionX * (*speedX)[*playerId];
    const float dirY = directionY * (*speedY)[*playerId];

    (*positionX)[*playerId] += dirX * *secondPerFrame;
    (*positionY)[*playerId] += dirY * *secondPerFrame;
 
    (*dirtyPosition)[*playerId] = true;
  }
}

void Player::camHandle()
{
  if ( *follow == true ) 
  {
    (*camPositionX)[0] = (*positionX)[*playerId];
    (*camPositionY)[0] = (*positionY)[*playerId];
    (*camPositionZ)[0] = (*positionZ)[*playerId];

    (*camPositionX)[1] = 0.6;
    (*camPositionY)[1] = 0.6;
    (*camPositionZ)[1] = 0.6;

    (*camPositionX)[2] = 0;
    (*camPositionY)[2] = 0;
    (*camPositionZ)[2] = 0;

    *camDirty = *camDirty || true;
  }
  else
  {
    zoom = (*camPositionX)[1];
    zoom = (*camPositionY)[1];
    zoom = (*camPositionZ)[1];

    posOffset.x =  (*camPositionX)[0] - (*positionX)[*playerId];
    posOffset.y =  (*camPositionY)[0] - (*positionY)[*playerId];
    posOffset.z =  (*camPositionZ)[0] - (*positionZ)[*playerId];

  }

}

void Player::loadData( ObjectDataRW objectDataRW, DirtyData dirtyData, SDL_Window* window, CameraDataRW cameraDataRW)
{
  this->window         = window                     ;
  this->playerId       = objectDataRW.playerId      ;
  this->exist          = objectDataRW.exist         ;
  this->secondPerFrame = objectDataRW.secondPerFrame;
  this->positionX      = objectDataRW.positionX     ;
  this->positionY      = objectDataRW.positionY     ;
  this->positionZ      = objectDataRW.positionZ     ;
  this->scalingX       = objectDataRW.scalingX      ;
  this->scalingY       = objectDataRW.scalingY      ;
  this->scalingZ       = objectDataRW.scalingZ      ;
  this->rotationX      = objectDataRW.rotationX     ;
  this->rotationY      = objectDataRW.rotationY     ;
  this->rotationZ      = objectDataRW.rotationZ     ;
  this->speedX         = objectDataRW.speedX        ;
  this->speedY         = objectDataRW.speedY        ;
  this->dirtyPosition  = dirtyData.dirtyPosition    ;
  this->dirtyScaling   = dirtyData.dirtyScaling     ;
  this->dirtyScaling   = dirtyData.dirtyRotation    ;

  this->follow        = cameraDataRW.follow        ;
  this->camDirty      = cameraDataRW.dirty         ;

  this->camPositionX  = cameraDataRW.positionX     ;
  this->camPositionY  = cameraDataRW.positionY     ;
  this->camPositionZ  = cameraDataRW.positionZ     ;
  this->rotateTranpose= cameraDataRW.rotateTranpose;
}