#include "_world.h"



ObjectDataRO World::readOnlyData()  
{ 
  return 
  { 
    &secondPerFrame,
    &playerId      ,
    &type          ,
    &imageId       ,
    &transparency  ,
    &exist         ,
    &dimension,
    &speedX   ,
    &speedY   ,  
    &positionX ,
    &positionY ,
    &positionZ , 
    &scalingX  ,
    &scalingY  ,
    &scalingZ  ,
    &rotationX ,
    &rotationY ,
    &rotationZ ,
    &velocityX ,
    &velocityY ,
    &velocityZ ,
    &scaleX    ,
    &scaleY    ,
    &scaleZ    ,
    &rotateX   ,
    &rotateY   ,
    &rotateZ   
  }; 
}; 


ObjectDataRW World::readWriteData()
{
  return
  {
    &secondPerFrame,
    &playerId      ,
    &type          ,
    &imageId       ,
    &transparency  ,
    &exist         ,
    &dimension,
    &speedX   ,
    &speedY   ,  
    &positionX ,
    &positionY ,
    &positionZ , 
    &scalingX  ,
    &scalingY  ,
    &scalingZ  ,
    &rotationX ,
    &rotationY ,
    &rotationZ ,
    &velocityX ,
    &velocityY ,
    &velocityZ ,
    &scaleX    ,
    &scaleY    ,
    &scaleZ    ,
    &rotateX   ,
    &rotateY   ,
    &rotateZ   
    };
}

DirtyData World::dirtyData()
{
  return
  {
  &dirtyImage   ,
  &dirtyPosition,
  &dirtyScaling ,
  &dirtyRotation,
  &dirtyVelocity,
  &dirtyScale   ,
  &dirtyRotate  
  };
}