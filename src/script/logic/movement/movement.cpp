#include "movement.h"
#include <iostream>
void Movement::move( bool nn )
{
  
  if (positionX->size() > lastsize)
  {
  for (size_t i = lastsize; i < positionX->size(); i++)
      sim.addAgent(RVO::Vector3((*positionX)[i], (*positionY)[i], (*positionZ)[i]));
    lastsize = positionX->size();
  }
   const size_t count = positionX->size();

  if (nn)
  {

    for ( int a = 0; a < dirtyVelocity->size(); a++)
    {
    sim.setAgentPosition(a, RVO::Vector3((*positionX)[a], (*positionY)[a], (*positionZ)[a]));

      if (a == *playerId) 
      {
      sim.setAgentPrefVelocity(a, RVO::Vector3((*velocityX)[a], (*velocityY)[a], (*velocityZ)[a]));

        continue;
      }


       sim.setAgentPrefVelocity(a, RVO::Vector3(0.f, 0.f, 0.f));

    } 

    sim.doStep();   // the thing you want to measure

    for (size_t i = 0; i < count; i++)
    {
      if (i == *playerId) continue;

      RVO::Vector3 v = sim.getAgentVelocity(i);
      (*velocityX)[i] = v.x();
      (*velocityY)[i] = v.y();
      (*velocityZ)[i] = v.z();
      (*dirtyVelocity)[i] = true;
    }
    return;
  }
  if (dirtyVelocity->size() <= 0) return;

  for (size_t i = 0; i < count; i++)
  {
    sim.setAgentPosition(i, RVO::Vector3((*positionX)[i], (*positionY)[i], (*positionZ)[i]));

    if (i == *playerId)
    {
      sim.setAgentPrefVelocity(i, RVO::Vector3((*velocityX)[i], (*velocityY)[i], (*velocityZ)[i]));
      continue;
    }

    float dx = 0 - (*positionX)[i];
    float dy = 0 - (*positionY)[i];
    float dz = (*positionZ)[*playerId] - (*positionZ)[i];
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist > stopDist)
      sim.setAgentPrefVelocity(i, RVO::Vector3(dx / dist * (*speedX)[i],
                                               dy / dist * (*speedY)[i],
                                               dz / dist * 0));
    else
      sim.setAgentPrefVelocity(i, RVO::Vector3(0.f, 0.f, 0.f));
  }


sim.doStep();   // the thing you want to measure

  for (size_t i = 0; i < count; i++)
  {
    if (i == *playerId) continue;

    RVO::Vector3 v = sim.getAgentVelocity(i);
    (*velocityX)[i] = v.x();
    (*velocityY)[i] = v.y();
    (*velocityZ)[i] = v.z();
    (*dirtyVelocity)[i] = true;
  }
}


void Movement::loadData(MovementStructData data)
{
  this->playerId      = data.playerID ;
  this->deltaTime     = data.deltaTime;
  this->positionX     = data.positionX;
  this->positionY     = data.positionY;
  this->positionZ     = data.positionZ;
  this->dirtyVelocity = data.dirtyVelocity;
  this->velocityX     = data.velocityX;
  this->velocityY     = data.velocityY;
  this->velocityZ     = data.velocityZ;
  this->speedX        = data.speedX;
  this->speedY        = data.speedY;
}