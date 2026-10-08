#ifndef WORLD_H
#define WORLD_H

#include <SDL3/SDL.h>

#include <vector>
#include <glm/glm.hpp>

#include <game/renderObject/enum/enum.h>
#include <game/renderObject/enum/struct.h>

class World
{
  public:
    World();
    const RenderStoreResource rendererResourceStore() const; 
    const RenderStoreResource readData() const;
    RenderObjectPosition savePosition() ;
    RenderVelocity getVelocity();
    const std::vector<bool>& dirtyObject() const { return dirty;};
    void createObject(Type objectType);
    void velocityUpdate();
    // void 

    void secondPerFrame();

  private:
    float lastFrame;
    float secondFrame;
    
    int8_t lastAxisX = 0;
    int8_t lastAxisY = 0;

    float holdtime = 0.0f;

    std::vector<Type> type          ;
    std::vector<bool> dirty         ;

    std::vector<float> positionX    ;
    std::vector<float> positionY    ;
    std::vector<float> positionZ    ;

    std::vector<float> scaleX       ;
    std::vector<float> scaleY       ;
    std::vector<float> scaleZ       ;

    std::vector<float> rotationX    ;
    std::vector<float> rotationY    ;
    std::vector<float> rotationZ    ;

    std::vector<float> velocityX    ;
    std::vector<float> velocityY    ;
    std::vector<float> velocityZ    ;

    std::vector<float> speedX       ;
    std::vector<float> speedY       ;

    std::vector<bool> visible       ;
    std::vector<bool> exist         ;

};

#endif