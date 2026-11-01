#include "movement.h"
#include <cmath>
#include <iostream>
struct Box { float cx, cy, hw, hh, angle; };
bool boxesCollide(const Box& A, const Box& B);

void Movement::updateCell()
{


  size_t currentCount = positionX->size();
  if ( currentCount <= 0) return;

  cell.clear();
  glm::vec3 center = getCenter();

  int camX = (int)floorf(center.x / gridsize);
  int camY = (int)floorf(center.y / gridsize);

  const float minX = (camX - updateSize)     * gridsize;
  const float maxX = (camX + updateSize + 1) * gridsize;
  const float minY = (camY - updateSize)     * gridsize;
  const float maxY = (camY + updateSize + 1) * gridsize;

  for (size_t i = 0; i < positionX->size(); i++)
  {
    float x = (*positionX)[i], y = (*positionY)[i];
    if (x < minX || x >= maxX || y < minY || y >= maxY) continue;
    cell[(int)floorf(x / gridsize)][(int)floorf(y / gridsize)].push_back((int)i);
  }
}

void Movement::checkCollide()
{

  int currentCount = positionX->size();
  if ( currentCount <= 0) return;

  if (currentCount > lastSize)
  {
    for (int a = lastSize; a < currentCount; a++)
    {
      nn.push_back({0, 0});
      animatingCount.push_back(step);
      isAnimating.push_back(false);
    }
    lastSize = currentCount;
  }
  for (auto& [gx, column] : cell)
  {
    for (auto& [gy, list] : column)
    {
      for (int i : list)
      {
        int tempx = (int)floorf((*positionX)[i] / gridsize);
        int tempy = (int)floorf((*positionY)[i] / gridsize);
        for (int a = tempx - checksize; a <= tempx + checksize; a++)
        {
          auto itX = cell.find(a);                 // look, never create
          if (itX == cell.end()) continue;
          for (int b = tempy - checksize; b <= tempy + checksize; b++)
          {
            auto itY = itX->second.find(b);
            if (itY == itX->second.end()) continue;
            for ( int d : itY->second)
            {
              Box A = {(*positionX)[i], (*positionY)[i], (*dimension)[i].width / 2, (*dimension)[i].height / 2, 0};
              Box B = {(*positionX)[d], (*positionY)[d], (*dimension)[d].width / 2, (*dimension)[d].height / 2, 0};

              if (i > d && boxesCollide( A, B )) 
              {
  
                float difX = A.cx - B.cx;
                float difY = A.cy - B.cy;
                float len  = sqrtf(difX * difX + difY * difY);
                float nxx = 1.f, nyy = 0.f;                        // fallback if centers are identical
                if (len > 0.0001f) { nxx = difX / len; nyy = difY / len; }

                nn[i] = {  nxx,  nyy };                            // a moves away from b
                nn[d] = { -nxx, -nyy };                            // b moves away from a
                isAnimating[i] = true;
                isAnimating[d] = true;
              }  
            }
          }  
        }
      }
    }
  }
}
void Movement::doAnimate()
{
  int currentCount = positionX->size();
  if ( currentCount <= 0) return;

  for (int a = 0; a < positionX->size(); a++)
  {
    if (isAnimating[a] == false) continue;
    if (animatingCount[a] <= 0 ) 
    {
      animatingCount[a] = step;
      isAnimating[a] = false;
      continue;
    }
    animatingCount[a]--;
    (*positionX)[a] += nn[a].x * 100 * *deltaTime;
    (*positionY)[a] += nn[a].y * 100 * *deltaTime; 
    (*dirtyVelocity)[a] = true;
  }
}

bool boxesCollide(const Box& A, const Box& B)
{
  
  // the two edge directions of each box
  float aux =  cosf(A.angle), auy = sinf(A.angle);    // A's "right"
  float avx = -sinf(A.angle), avy = cosf(A.angle);    // A's "down"
  float bux =  cosf(B.angle), buy = sinf(B.angle);
  float bvx = -sinf(B.angle), bvy = cosf(B.angle);

  float dx = B.cx - A.cx;                              // center to center
  float dy = B.cy - A.cy;

  float axes[4][2] = { {aux, auy}, {avx, avy}, {bux, buy}, {bvx, bvy} };

  for (int i = 0; i < 4; i++)
  {
    float ax = axes[i][0], ay = axes[i][1];

    // how far each box reaches along this axis
    float rA = A.hw * fabsf(aux * ax + auy * ay) + A.hh * fabsf(avx * ax + avy * ay);
    float rB = B.hw * fabsf(bux * ax + buy * ay) + B.hh * fabsf(bvx * ax + bvy * ay);

    float dist = fabsf(dx * ax + dy * ay);             // centers' distance along the axis

    if (dist > rA + rB) return false;                  // gap found, so no collision
  }
  return true;                                         // no gap on any axis
}
void Movement::loadData( ObjectData objectData, MovementStructData data, Position cameraPosition)
{

  this->playerId      = objectData.playerId  ;

  this->positionX      = objectData.positionX;
  this->positionY      = objectData.positionY;
  this->positionZ      = objectData.positionZ;

  this->scalingX       = objectData.scalingX;
  this->scalingY       = objectData.scalingY;
  this->scalingZ       = objectData.scalingZ;

  this->rotationX      = objectData.rotationX;
  this->rotationY      = objectData.rotationY;
  this->rotationZ      = objectData.rotationZ;

  this->deltaTime     = data.deltaTime;
  this->dimension     = data.dimension;

  this->dirtyVelocity = data.dirtyVelocity;
  this->velocityX     = data.velocityX;
  this->velocityY     = data.velocityY;
  this->velocityZ     = data.velocityZ;
  this->speedX        = data.speedX;
  this->speedY        = data.speedY;

  this->follow       = cameraPosition.follow;
  this->camPositionX = cameraPosition.positionX;
  this->camPositionY = cameraPosition.positionY;
  this->camPositionZ = cameraPosition.positionZ;
}

glm::vec3 Movement::getCenter()
{
  if (*follow == 1u)
  {
    (*camPositionX)[0] = (*positionX)[*playerId];
    (*camPositionY)[0] = (*positionY)[*playerId];
    (*camPositionZ)[0] = (*positionZ)[*playerId];
    return {(*camPositionX)[0], (*camPositionY)[0], (*camPositionZ)[0]};
  }
  else if (*follow == 0u)
  {
    return {(*camPositionX)[0], (*camPositionY)[0], (*camPositionZ)[0]};
  }
  return {};
}
