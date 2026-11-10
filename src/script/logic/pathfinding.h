#pragma once

#include <struct/struct.h>

class PathFinding
{
  private:
    Nodes nodes;
    std::unordered_map<uint64_t, std::vector<glm::vec3>> tasks;

    float *secondPerFrame;

    std::vector<bool>  *dirtyPosition;
    std::vector<float> *positionX    ;
    std::vector<float> *positionY    ;
    std::vector<float> *positionZ    ;

    std::vector<float> *speedX       ;
    std::vector<float> *speedY       ;    
  public:
    PathFinding();
    void reconstructPath(Node* current, uint64_t id);
    void findPath( float startX, float startY, float goalX, float goalY, uint64_t id);
    float heuristic(Node& A, Node& B);
    void loadData(ObjectDataRW objectDataRW, DirtyData dirtyData);
    void animate();
};