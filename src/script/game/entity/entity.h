#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <struct/struct.h>

class Entity
{
  private:
    std::unordered_map<std::string, int> typeIDs;
    int entityIdCount = 0;
  public:
    const typeIDsData getTypeIDs() const {return &typeIDs;}
    Entity();
    void assign();
};
