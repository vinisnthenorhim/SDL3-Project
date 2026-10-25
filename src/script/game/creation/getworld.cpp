#include "_world.h"



ObjectData World::readData()  
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

MovementStructData World::getMovementData()
{
  return
  {
    &secondFrame  ,
    &positionX    ,
    &positionY    ,
    &positionZ    ,
    &dirtyVelocity,
    &velocityX    ,
    &velocityY    ,
    &velocityZ    ,
    &speedX       ,
    &speedY       
  };
}