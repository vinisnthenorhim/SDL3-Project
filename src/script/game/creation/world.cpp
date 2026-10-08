#include <iostream>
#include <iomanip>

#include "_world.h"

World::World()
{
  lastFrame = SDL_GetTicksNS();
  std::cout << std::fixed << std::setprecision(6);
}

const RenderStoreResource World::readData() const 
{ 
  return 
  { 
    &type, 
    &visible,
    &speedX, 
    &speedY,
    &positionX,
    &positionY,
    &positionZ,
    &scaleX,
    &scaleY,
    &scaleZ,    
    &rotationX,
    &rotationY,
    &rotationZ,
  }; 
}; 

RenderObjectPosition World::savePosition()
{
  return 
  { 
    &positionX,
    &positionY,
    &positionZ,
    &scaleX,
    &scaleY,
    &scaleZ,    
    &rotationX,
    &rotationY,
    &rotationZ,
  }; 
}
RenderVelocity World::getVelocity()
{
  return
  {
    &secondFrame,
    &dirty,
    &velocityX,
    &velocityY,
    &velocityZ
  };
}

void World::createObject(Type objectType)
{

  type.push_back(objectType);
  dirty.push_back(false)    ;

  positionX.push_back(0.0f);
  positionY.push_back(0.0f);
  positionZ.push_back(0.0f);

  scaleX.push_back(1.0f)   ;
  scaleY.push_back(1.0f)   ;
  scaleZ.push_back(1.0f)   ;

  rotationX.push_back(0.0f);
  rotationY.push_back(0.0f);
  rotationZ.push_back(0.0f);

  velocityX.push_back(0.0f);
  velocityY.push_back(0.0f);
  velocityZ.push_back(0.0f);

  speedX.push_back(200.0f)   ;
  speedY.push_back(350.0f)   ;

  visible.push_back(true)  ;
  exist.push_back(true)    ;

  std::cout << "Object No: " << type.size() << '\n';

}
void World::secondPerFrame()
{
  float currentFrame = SDL_GetTicksNS();

  secondFrame = ( currentFrame - lastFrame)/1000000000;
  lastFrame = currentFrame;
}



void World::velocityUpdate( )
{
  int playerId = -1; 
  const int size = exist.size();
  for (int a = 0; a < size; a++)
  {
    if (type[a] == Type::Player)
    {
      playerId = a;
      break;
    }
  }

  if (playerId == -1) return; 

  const bool* keyboardState = SDL_GetKeyboardState(NULL);

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

  const float newVelocityX = length > 0 ? axisX / length : 0.0f;
  const float newVelocityY = length > 0 ? axisY / length : 0.0f;

  const float dirX = newVelocityX * speedX[playerId];
  const float dirY = newVelocityY * speedY[playerId];

  // std::cout << dirX << '\t' << dirY << '\n';

  if (velocityX[playerId] != newVelocityX || velocityY[playerId] != newVelocityY || velocityX[playerId] != 0 || velocityY[playerId] != 0)
  {
    velocityX[playerId] = dirX;
    velocityY[playerId] = dirY;

    dirty[playerId] = true;
  }
  else
  {
    velocityX[playerId] = 0;
    velocityY[playerId] = 0;
  }

  // std::cout << velocityX[playerId] << '\t' << velocityY[playerId] << '\n';

}