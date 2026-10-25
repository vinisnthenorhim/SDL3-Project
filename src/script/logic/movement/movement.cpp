#include "movement.h"
#include <iostream>

void Movement::move(std::vector<float> curPos, std::vector<float> nowPos, float playPosX, float playPosy, bool nn)
{
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
      else 
      {
        (*dirtyVelocity)[a] = false;
      } 
    }
    return;
  }
  if (dirtyVelocity->size() <= 0) return;
  for ( int a = 0; a < dirtyVelocity->size(); a++)
  {
    if (a == *playerId) continue;

    float dx = (*positionX)[*playerId] - (*positionX)[a];
    float dy = (*positionY)[*playerId] - (*positionY)[a];
    if (!( fabsf(dx) < 1.0f && fabsf(dy) < 1.0f ))
    {
      const float length = std::sqrt(float( dx * dx + dy * dy));

      const float newVelocityX = length > 0 ? dx / length : 0.0f;
      const float newVelocityY = length > 0 ? dy / length : 0.0f;

      const float dirX = newVelocityX * (*speedX)[a];
      const float dirY = newVelocityY * (*speedY)[a];

      (*velocityX)[a] = dirX;
      (*velocityY)[a] = dirY;

      (*dirtyVelocity)[a] = true;
    }
    else if ((*velocityX)[a] != 0 || (*velocityY)[a] != 0)
    {
      (*velocityX)[a] = 0;
      (*velocityY)[a] = 0;

      (*dirtyVelocity)[a] = true;
    }
    else 
    {
      (*dirtyVelocity)[a] = false;
    }
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