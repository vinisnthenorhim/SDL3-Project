#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <glm/glm.hpp>

#include <loader/lua/BWlua.h>

#include <struct/struct.h>
#include <game/entity/entity.h>
class World
{
  public:
    World();
    ObjectDataRO readOnlyData();
    ObjectDataRW readWriteData();
    DirtyData    dirtyData();
    void loadSaveData();
    void createObject(CreateEntityLayout layout);
    void updateData();
    void create(SDL_Window* window);

    void frameCalculate();

    void saveData();

  private:
    Entity entity;
    typeIDsData entityId;

    sol::state creationstate = nullptr;

    float lastFrame;
    float secondPerFrame;
    
    int8_t lastAxisX = 0;
    int8_t lastAxisY = 0;

    float holdtime = 0.0f;

    uint64_t playerId = 0;

    std::vector<ObjectDimension> dimension;

    std::vector<int>   type          ;

    std::vector<bool>  dirtyImage    ;
    std::vector<int>   imageId       ;
    std::vector<float> transparency  ;
    std::vector<bool>  exist         ;

    std::vector<bool>  dirtyPosition;
    std::vector<float> positionX    ;
    std::vector<float> positionY    ;
    std::vector<float> positionZ    ;

    std::vector<bool>  dirtyScaling ;
    std::vector<float> scalingX     ;
    std::vector<float> scalingY     ;
    std::vector<float> scalingZ     ;

    std::vector<bool>  dirtyRotation;
    std::vector<float> rotationX    ;
    std::vector<float> rotationY    ;
    std::vector<float> rotationZ    ;

    std::vector<bool>  dirtyVelocity;
    std::vector<float> velocityX    ;
    std::vector<float> velocityY    ;
    std::vector<float> velocityZ    ;

    std::vector<bool>  dirtyScale   ;
    std::vector<float> scaleX       ;
    std::vector<float> scaleY       ;
    std::vector<float> scaleZ       ;

    std::vector<bool>  dirtyRotate  ;
    std::vector<float> rotateX      ;
    std::vector<float> rotateY      ;
    std::vector<float> rotateZ      ;

    std::vector<float> speedX       ;
    std::vector<float> speedY       ;

};
