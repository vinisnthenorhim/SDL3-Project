#include "entity.h"
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

Entity::Entity()
{ 
  BW::File file = {"../src/data/json/texture/atlas.json", "rb"};

  char buf[16384];
  rapidjson::FileReadStream is(file.fp, buf, sizeof(buf));

  rapidjson::Document entityFile;
  entityFile.ParseStream(is);

  if (!entityFile.HasMember("image")) {
    std::cout << "Entity FIle has no Member image\n";
    return;
  }
  const auto& names = entityFile["image"];

  for (const auto& name : names.GetArray())
  {
    typeIDs.emplace(name["type"].GetString(), entityIdCount++);
  }
}