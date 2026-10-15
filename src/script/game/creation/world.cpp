#include <iostream>
#include <iomanip>

#include "_world.h"

World::World()
{
  lastFrame = SDL_GetTicksNS();
  luaRuntime.lua().new_enum<Type>
  (
    "EntityType",
    {
      {"Player", Type::Player}
    }
  );
  luaRuntime.registerFunction("create_object", [this](Type type, float x, float y){createObject(type, x, y);});

  luaRuntime.loadScript("../src/script/creation/creation.lua");

}

ObjectData World::readData()  
{ 
  return 
  { 
    &visible,
    &speedX, 
    &speedY,
    &positionX,
    &positionY,
    &positionZ,
    &scalingX,
    &scalingY,
    &scalingZ,    
    &rotationX,
    &rotationY,
    &rotationZ,
  }; 
}; 


ObjectTransformData World::getObjectTranform()
{
  return
  {
    &secondFrame,
    &dirtyVelocity,
    &velocityX,
    &velocityY,
    &velocityZ,
    &dirtyScale,
    &scaleY,
    &scaleZ,
    &scaleX,
    &dirtyRotate,
    &rotateX,
    &rotateY,
    &rotateZ
  };
}
void World::create(SDL_Window* window)
{
  SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
  const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);


  sol::protected_function function = luaRuntime.lua_function("random_location_creation");

  function( mode->w, mode->h );
  std::cout << "called" << '\n';
}
void World::createObject(Type objectType, float locationX, float locationY)
{

  type.push_back(objectType);
  
  dirtyVelocity.push_back(false)  ; dirtyScale.push_back(false)  ;  dirtyRotate.push_back(false)  ; 

  positionX.push_back(locationX) ; positionY.push_back(locationY) ; positionZ.push_back(0.0f)  ;

  scalingX .push_back(1.0f) ; scalingY .push_back(1.0f) ; scalingZ .push_back(1.0f)  ;

  rotationX.push_back(0.0f) ; rotationY.push_back(0.0f) ; rotationZ.push_back(0.0f)  ;

  velocityX.push_back(0.0f) ; velocityY.push_back(0.0f) ; velocityZ.push_back(0.0f)  ;

  scaleX   .push_back(0.0f) ; scaleY   .push_back(0.0f) ; scaleZ   .push_back(0.0f)  ;

  rotateX  .push_back(0.0f) ; rotateY  .push_back(0.0f) ; rotateZ  .push_back(0.0f)  ;

  speedX.push_back(500.0f)  ;
  speedY.push_back(500.0f)  ;

  visible.push_back(true)   ;
  exist.push_back(true)     ;

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

  const float newVelocityX = length > 0 ? axisX / length : 0.0f;
  const float newVelocityY = length > 0 ? axisY / length : 0.0f;

  const float dirX = newVelocityX * speedX[playerId];
  const float dirY = newVelocityY * speedY[playerId];

  // std::cout << dirX << '\t' << dirY << '\n';

  if (velocityX[playerId] != newVelocityX || velocityY[playerId] != newVelocityY || velocityX[playerId] != 0 || velocityY[playerId] != 0)
  {
    velocityX[playerId] = dirX;
    velocityY[playerId] = dirY;

    dirtyVelocity[playerId] = true;
  }
  else
  {
    velocityX[playerId] = 0;
    velocityY[playerId] = 0;
    dirtyVelocity[playerId] = false;
  }

  // std::cout << velocityX[playerId] << '\t' << velocityY[playerId] << '\n';

}