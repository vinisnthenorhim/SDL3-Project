#include "separation.h"
#include <cmath>
#include <iostream>
struct Box { float cx, cy, hw, hh, angle; };
bool boxesCollide(const Box& A, const Box& B);

void ObjectSeparation::updateCell()
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

void ObjectSeparation::checkCollide()
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
  if ( cell.empty() ) return; // check if cell contain data or not
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
void ObjectSeparation::doAnimate()
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
    (*dirtyPosition)[a] = true;
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
void ObjectSeparation::loadData( ObjectDataRW objectDataRW, DirtyData dirtyData, CameraDataRW cameraDataRW)
{
  this->playerId      = objectDataRW.playerId      ;
  this->deltaTime     = objectDataRW.secondPerFrame;
  this->dimension     = objectDataRW.dimension     ;
  this->positionX     = objectDataRW.positionX     ;
  this->positionY     = objectDataRW.positionY     ;
  this->positionZ     = objectDataRW.positionZ     ;
  this->scalingX      = objectDataRW.scalingX      ;
  this->scalingY      = objectDataRW.scalingY      ;
  this->scalingZ      = objectDataRW.scalingZ      ;
  this->rotationX     = objectDataRW.rotationX     ;
  this->rotationY     = objectDataRW.rotationY     ;
  this->rotationZ     = objectDataRW.rotationZ     ;
  this->dirtyPosition = dirtyData.dirtyPosition    ;
  this->dirtyScaling  = dirtyData.dirtyScaling     ;
  this->dirtyScaling  = dirtyData.dirtyRotation    ;
  this->follow        = cameraDataRW.follow        ;
  this->camPositionX  = cameraDataRW.positionX     ;
  this->camPositionY  = cameraDataRW.positionY     ;
  this->camPositionZ  = cameraDataRW.positionZ     ;
}

glm::vec3 ObjectSeparation::getCenter()
{

  if (*follow == 1u && *playerId < positionX->size())
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
