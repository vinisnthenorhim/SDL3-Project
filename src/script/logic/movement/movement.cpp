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

  if (nn)
  {
    for ( int a = 0; a < dirtyVelocity->size(); a++)
    {
      if (a == *playerId) continue;

      if ((*velocityX)[a] != 0 || (*velocityY)[a] != 0)
      {
        (*velocityX)[a] = 0;
        (*velocityY)[a] = 0;

        (*dirtyVelocity)[a] = true;
      }
    } 
    return;
  }
  if (dirtyVelocity->size() <= 0) return;
   const size_t count = positionX->size();

  for (size_t i = 0; i < count; i++)
  {
    sim.setAgentPosition(i, RVO::Vector3((*positionX)[i], (*positionY)[i], (*positionZ)[i]));

    if (i == *playerId)
    {
      sim.setAgentPrefVelocity(i, RVO::Vector3((*velocityX)[i], (*velocityY)[i], (*velocityZ)[i]));
      continue;
    }

    float dx = (*positionX)[*playerId] - (*positionX)[i];
    float dy = (*positionY)[*playerId] - (*positionY)[i];
    float dz = (*positionZ)[*playerId] - (*positionZ)[i];
    float dist = std::sqrt(dx * dx + dy * dy + dz * dz);

    if (dist > stopDist)
      sim.setAgentPrefVelocity(i, RVO::Vector3(dx / dist * (*speedX)[i],
                                               dy / dist * (*speedY)[i],
                                               dz / dist * (*speedY)[i]));
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