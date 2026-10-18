#include <iostream>
#include <iomanip>
#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "_world.h"

World::World()
{
  lastFrame = SDL_GetTicksNS();

  luaRuntime.registerFunction("create_object", [this](CreateEntityLayout layout){createObject(layout);});
  luaRuntime.lua().new_usertype<CreateEntityLayout>
  (
    "Layout",
    sol::constructors<CreateEntityLayout(), CreateEntityLayout(float, float, float, float, float, float, float, float, float)>(),
    "posX", &CreateEntityLayout::posX,
    "posY", &CreateEntityLayout::posY,
    "posZ", &CreateEntityLayout::posZ,
    "sclX", &CreateEntityLayout::sclX,
    "sclY", &CreateEntityLayout::sclY,
    "sclZ", &CreateEntityLayout::sclZ,
    "rotX", &CreateEntityLayout::rotX,
    "rotY", &CreateEntityLayout::rotY,
    "rotZ", &CreateEntityLayout::rotZ
  );

  luaRuntime.loadScript("../src/script/creation/creation.lua");

  if (save) loadSaveData();



}

void World::loadSaveData()
{
  FILE* fp = fopen("../src/script/data/games/save.json", "rb");

  if (!fp) return;

  char buf[65536];
  rapidjson::FileReadStream is(fp, buf, sizeof(buf));

  rapidjson::Document doc;
  doc.ParseStream(is);
  fclose(fp);

  if (!doc.HasMember("position") || !doc["position"].IsObject() || !doc.HasMember("scale") || !doc["scale"].IsObject() || !doc.HasMember("rotation") || !doc["rotation"].IsObject()) return;
  const auto& pos = doc["position"];
  const auto& scl = doc["scale"];
  const auto& rot = doc["rotation"];

  for( int a = 0; a < doc["count"].GetInt(); a++)
  {
    createObject({ pos["x"][a].GetFloat(), pos["y"][a].GetFloat(), pos["z"][a].GetFloat(), scl["x"][a].GetFloat(), scl["y"][a].GetFloat(), scl["z"][a].GetFloat(), rot["x"][a].GetFloat(), rot["y"][a].GetFloat(),  rot["z"][a].GetFloat() });
  }
}

void World::create(SDL_Window* window)
{
  SDL_DisplayID displayID = SDL_GetDisplayForWindow(window);
  const SDL_DisplayMode* mode = SDL_GetDesktopDisplayMode(displayID);


  sol::protected_function function = luaRuntime.lua_function("random_location_creation");

  function( mode->w, mode->h );
}

void World::createObject(CreateEntityLayout layout)
{
  
  dirtyVelocity.push_back(false)  ; dirtyScale.push_back(false)  ;  dirtyRotate.push_back(false)  ; 

  positionX.push_back(layout.posX) ; positionY.push_back(layout.posY) ; positionZ.push_back(layout.posZ)  ;

  scalingX .push_back(1.0f) ; scalingY .push_back(1.0f) ; scalingZ .push_back(1.0f)  ;

  rotationX.push_back(0.0f) ; rotationY.push_back(0.0f) ; rotationZ.push_back(0.0f)  ;

  velocityX.push_back(0.0f) ; velocityY.push_back(0.0f) ; velocityZ.push_back(0.0f)  ;

  scaleX   .push_back(0.0f) ; scaleY   .push_back(0.0f) ; scaleZ   .push_back(0.0f)  ;

  rotateX  .push_back(0.0f) ; rotateY  .push_back(0.0f) ; rotateZ  .push_back(0.0f)  ;

  speedX.push_back(500.0f)  ;
  speedY.push_back(500.0f)  ;

  visible.push_back(true)   ;
  exist.push_back(true)     ;

  std::cout << "Object No: " << visible.size() << '\n';
}
void World::secondPerFrame()
{
  float currentFrame = SDL_GetTicksNS();

  secondFrame = ( currentFrame - lastFrame)/1000000000;
  lastFrame = currentFrame;
}



void World::velocityUpdate( )
{
  if (visible.size() <= 0) return;
  int playerId = 0; 

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