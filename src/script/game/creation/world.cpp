#include <iostream>
#include <iomanip>
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

#include "_world.h"

World::World()
{
  lastFrame = SDL_GetTicksNS();
  entityId = entity.getTypeIDs();


  creationstate.set_function("create_object", [this](CreateEntityLayout layout){createObject(layout);});
  creationstate.new_usertype<CreateEntityLayout>
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
  BW::stateLoader(creationstate, "../src/script/creation/creation.lua");

  BW::File fp = {"../src/data/json/games/save.json", "rb"};


  char buf[65536];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document load;
  load.ParseStream(is);

  if (load["load"].GetBool()) loadSaveData();

}

void World::loadSaveData()
{
  
  BW::File fp = {"../src/data/json/games/entitydata.json", "rb"};


  char buf[65536];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document doc;
  doc.ParseStream(is);

  if (!doc.HasMember("position") || !doc["position"].IsObject() || !doc.HasMember("scale") || !doc["scale"].IsObject() || !doc.HasMember("rotation") || !doc["rotation"].IsObject()) 
  {

    return;
  }

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


  sol::protected_function function = creationstate["random_location_creation"];

  function( mode->w, mode->h );
  // createObject({0});
}

void World::createObject(CreateEntityLayout layout)
{
  type.push_back(entityId->at("projectile"));
  
  dirtyVelocity.push_back(false)  ; dirtyScale.push_back(false)  ;  dirtyRotate.push_back(false)  ; 

  positionX.push_back(layout.posX) ; positionY.push_back(layout.posY) ; positionZ.push_back(layout.posZ)  ;

  scalingX .push_back(1.0f) ; scalingY .push_back(1.0f) ; scalingZ .push_back(1.0f)  ;

  rotationX.push_back(0.0f) ; rotationY.push_back(0.0f) ; rotationZ.push_back(0.0f)  ;

  velocityX.push_back(0.0f) ; velocityY.push_back(0.0f) ; velocityZ.push_back(0.0f)  ;

  scaleX   .push_back(0.0f) ; scaleY   .push_back(0.0f) ; scaleZ   .push_back(0.0f)  ;

  rotateX  .push_back(0.0f) ; rotateY  .push_back(0.0f) ; rotateZ  .push_back(0.0f)  ;

  if (speedX.size() == 0)
  {
    speedX.push_back(500.0f)  ;
    speedY.push_back(500.0f)  ;
  }
  else
  {
    speedX.push_back(300.0f)  ;
    speedY.push_back(300.0f)  ;
  }


  visible.push_back(true)   ;
  exist.push_back(true)     ;

  std::cout << "Object No: " << visible.size() << " entity type: " << type[visible.size()-1]<< '\n';
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

  const float dirX = newVelocityX * speedX[playerID];
  const float dirY = newVelocityY * speedY[playerID];


  if (velocityX[playerID] != newVelocityX || velocityY[playerID] != newVelocityY || velocityX[playerID] != 0 || velocityY[playerID] != 0)
  {
    velocityX[playerID] = dirX;
    velocityY[playerID] = dirY;


    dirtyVelocity[playerID] = true;
  }
  else
  {
    velocityX[playerID] = 0;
    velocityY[playerID] = 0;
    dirtyVelocity[playerID] = false;
  }
}

void World::updateData()
{
  for (int a = 0; a < visible.size(); a++)
  {
    if (dirtyVelocity[a] == true)
    {
      positionX[a] += velocityX[a] * secondFrame;
      positionY[a] += velocityY[a] * secondFrame;
      positionZ[a] += velocityZ[a] * secondFrame;
    }
    if (dirtyScale[a] == true)
    {
      scalingX[a] += scaleX[a] * secondFrame;
      scalingY[a] += scaleY[a] * secondFrame;
      scalingZ[a] += scaleZ[a] * secondFrame;
    }
    if (dirtyRotate[a] == true)
    {
      rotationX[a] += rotateX[a] * secondFrame;
      rotationY[a] += rotateY[a] * secondFrame;
      rotationZ[a] += rotateZ[a] * secondFrame;
    }
  }
}
