#include <iostream>
#include <iomanip>
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
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

  float x = positionX.size() > playerId ? positionX[playerId] : 0 ;
  float y = positionY.size() > playerId ? positionY[playerId] : 0 ;

  function( mode->w, mode->h, static_cast<int>(x), static_cast<int>(y));
}

void World::createObject(CreateEntityLayout layout)
{
  type.push_back(entityId->at("player"));
  imageId.push_back(entityId->at("player"));

  transparency.push_back(1.0);
  exist.push_back(true)      ;

  dimension.push_back({60, 60});
  dirtyImage.push_back(true);

  dirtyPosition.push_back(true)  ; dirtyScaling.push_back(true); dirtyRotation.push_back(true) ;
  dirtyVelocity.push_back(true)  ; dirtyScale.push_back(true)  ;  dirtyRotate.push_back(true)  ; 

  positionX.push_back(layout.posX) ; positionY.push_back(layout.posY) ; positionZ.push_back(layout.posZ)  ;

  scalingX .push_back(1.0f) ; scalingY .push_back(1.0f) ; scalingZ .push_back(1.0f)  ;

  rotationX.push_back(0.0f) ; rotationY.push_back(0.0f) ; rotationZ.push_back(0.0f)  ;

  velocityX.push_back(0.0f) ; velocityY.push_back(0.0f) ; velocityZ.push_back(0.0f)  ;

  scaleX   .push_back(0.0f) ; scaleY   .push_back(0.0f) ; scaleZ   .push_back(0.0f)  ;

  rotateX  .push_back(0.0f) ; rotateY  .push_back(0.0f) ; rotateZ  .push_back(0.0f)  ;

  if (speedX.size() == 0)
  {
    speedX.push_back(1000.0f)  ;
    speedY.push_back(1000.0f)  ;
  }
  else
  {
    speedX.push_back(300.0f)  ;
    speedY.push_back(300.0f)  ;
  }

  std::cout << "Object No: " << exist.size() << " entity type: " << type[exist.size()-1]<< '\n';
}
void World::frameCalculate()
{
  float currentFrame = SDL_GetTicksNS();

  secondPerFrame = ( currentFrame - lastFrame)/1000000000;
  lastFrame = currentFrame;
}



void World::updateData()
{
  for (int a = 0; a < exist.size(); a++)
  {
    // if (dirtyVelocity[a] == true)
    // {
      positionX[a] += velocityX[a] * secondPerFrame;
      positionY[a] += velocityY[a] * secondPerFrame;
      positionZ[a] += velocityZ[a] * secondPerFrame;
    // }
    // if (dirtyScale[a]   == true)
    // {
      scalingX[a]  += scaleX[a]    * secondPerFrame;
      scalingY[a]  += scaleY[a]    * secondPerFrame;
      scalingZ[a]  += scaleZ[a]    * secondPerFrame;
    // }
    // if (dirtyRotate[a]  == true)
    // {
      rotationX[a] += rotateX[a]   * secondPerFrame;
      rotationY[a] += rotateY[a]   * secondPerFrame;
      rotationZ[a] += rotateZ[a]   * secondPerFrame;
    // }
  }
}


void World::saveData()
{
  BW::File fp = {"../src/data/json/games/save.json", "rb"};


  char buf[65536];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document load;
  load.ParseStream(is);


  if (!load["save"].GetBool()) return;

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
  
  w.SetIndent(' ', 2);
  w.StartObject();
  w.Key("count");
  w.Uint64(exist.size());
  w.Key("position");
  w.StartObject();
  w.Key("x");
  w.StartArray();

  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);
  for (int a = 0; a < exist.size(); a ++)
    w.Double(positionX[a]);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(positionY[a]);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(positionZ[a]);
  w.EndArray();
  w.EndObject();

  w.Key("scale");
  w.StartObject();

  w.Key("x");
  w.StartArray();
  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);

  for (int a = 0; a < exist.size(); a ++)
    w.Double(scalingX[a]);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(scalingY[a]);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(scalingZ[a]);
  w.EndArray();
  w.EndObject();

  w.Key("rotation");
  w.StartObject();

  w.Key("x");
  w.StartArray();
  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);
  for (int a = 0; a < exist.size(); a ++)
    w.Double(rotationX[a]);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(rotationY[a]);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < exist.size(); a ++)
    w.Double(rotationZ[a]);
  w.EndArray();
  w.EndObject();

  w.EndObject();
  BW::File fpw = {"../src/data/json/games/entitydata.json", "wb"};
  if (fpw.fp)
  {
    fwrite(buffer.GetString(), 1, buffer.GetSize(), fpw.fp);
  }
}