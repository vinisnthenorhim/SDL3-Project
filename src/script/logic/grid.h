#pragma once
#include <struct/struct.h>

class GridCell
{
  private:
    Grid cell;
    const int gridsize = 100;
    const int updateSize = 60;
    const int checksize = 1;
    const int step = 10;

    const std::vector<float> *positionX    ;
    const std::vector<float> *positionY    ;
    const std::vector<float> *positionZ    ;

    std::array<float, 4> *camPositionX ;
    std::array<float, 4> *camPositionY ;
    std::array<float, 4> *camPositionZ ;

    const uint64_t* playerId;
    bool* follow;

    glm::vec3 getCenter();

  public:
    void loadData( ObjectDataRO objectDataRO, CameraDataRW cameraDataRW );
    void updateCell();
    const Grid* getCell() const;
};