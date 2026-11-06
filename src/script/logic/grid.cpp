#include "grid.h"


void GridCell::updateCell()
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


glm::vec3 GridCell::getCenter()
{
  return {(*camPositionX)[0], (*camPositionY)[0], (*camPositionZ)[0]};
}

void GridCell::loadData( ObjectDataRO objectDataRO, CameraDataRW cameraDataRW )
{
  this->playerId      = objectDataRO.playerId      ;
  this->positionX     = objectDataRO.positionX     ;
  this->positionY     = objectDataRO.positionY     ;
  this->positionZ     = objectDataRO.positionZ     ;
  this->follow        = cameraDataRW.follow        ;
  this->camPositionX  = cameraDataRW.positionX     ;
  this->camPositionY  = cameraDataRW.positionY     ;
  this->camPositionZ  = cameraDataRW.positionZ     ;
}

const Grid* GridCell::getCell() const 
{
  return &cell;
}