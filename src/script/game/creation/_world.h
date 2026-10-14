#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <glm/glm.hpp>

#include <lua/LuaRuntime.h>

#include <game/renderObject/enum/enum.h>
#include <game/renderObject/enum/struct.h>

class World
{
  public:
    World();
    ObjectData readData();
    ObjectTransformData getObjectTranform();
    void createObject(Type objectType, float locationX, float locationY);
    void velocityUpdate();
    void create(int width, int height);

    void secondPerFrame();

  private:
    LuaRuntime luaRuntime;

    float lastFrame;
    float secondFrame;
    
    int8_t lastAxisX = 0;
    int8_t lastAxisY = 0;

    float holdtime = 0.0f;

    std::vector<Type> type          ;

    std::vector<float> positionX    ;
    std::vector<float> positionY    ;
    std::vector<float> positionZ    ;

    std::vector<float> scalingX     ;
    std::vector<float> scalingY     ;
    std::vector<float> scalingZ     ;

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

    std::vector<bool> visible       ;
    std::vector<bool> exist         ;

};
