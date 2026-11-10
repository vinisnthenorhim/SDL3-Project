#include <queue>
#include <algorithm>
#include "pathfinding.h"
PathFinding::PathFinding()
{
  for (int x = -500; x < 500; x++)
  {
    for (int y = -500; y < 500; y++)
    {
      nodes[x][y].x = x;
      nodes[x][y].y = y;
    }
  }
}
void PathFinding::loadData(ObjectDataRW objectDataRW, DirtyData dirtyData)
{
  dirtyPosition  = dirtyData.dirtyPosition;
  secondPerFrame = objectDataRW.secondPerFrame  ;
  positionX      = objectDataRW.positionX       ;
  positionY      = objectDataRW.positionY       ;
  positionZ      = objectDataRW.positionZ       ;
  speedX         = objectDataRW.speedX          ;
  speedY         = objectDataRW.speedY          ;    
}

void PathFinding::findPath( float startX, float startY, float goalX, float goalY, uint64_t id)
{
  if (nodes.empty()) return ;
  int sx = static_cast<int>( std::floor( startX / Node::w ) );
  int sy = static_cast<int>( std::floor( startY / Node::h ) );
  int ex = static_cast<int>( std::floor( goalX / Node::w ) );
  int ey = static_cast<int>( std::floor( goalY / Node::h ) );
  int height = static_cast<int>(nodes.size());
  int width = static_cast<int>(nodes[0].size());

  if (sx <= -width  /2 || sx >= width  /2 || sy <= -height /2 || sy >= height /2 || ex <= -width  /2 || ex >= width  /2 || ey <= -height /2 || ey >= height /2 ) return ;
  for (auto& [gx, column] : nodes)
  {
      for (auto& [gy, node] : column)
      {
          node.gx = std::numeric_limits<float>::infinity();
          node.hx = 0.0f;
          node.parent = nullptr;
          node.closed = false;
      }
  }
  Node* start = &(nodes[sx][sy]);
  Node* goal  = &(nodes[ex][ey]);

  std::priority_queue< Node*, std::vector<Node*>, CompareNode > openList;

  start->gx = 0.0f;
  start->hx = heuristic(*start, *goal);

  openList.push(start);

  constexpr int directions[8][2] =
  {
      {  0,  1 },
      {  1,  1 },
      {  1,  0 },
      {  1, -1 },
      {  0, -1 },
      { -1, -1 },
      { -1,  0 },
      { -1,  1 }
  };


  while ( !openList.empty() )
  {
    auto current = openList.top();
    openList.pop();

    if (current->closed == true) continue;
    if (current == goal) 
    {
      reconstructPath(current, id);
      return ;
    }
    current->closed = true;
    for (const auto& direction : directions)
    {
      int nx = current->x + direction[0];
      int ny = current->y + direction[1];

      if (nx <= -width  /2 || nx >= width  /2 || ny <= -height /2 || ny >= height /2 ) continue;

      Node* neighbor = &(nodes[nx][ny]);

      if (!neighbor->walkable || neighbor->closed) continue;

      float value = std::sqrtf( direction[0] * direction[0] + direction[1] * direction[1] );

      float tentativeG = current->gx + value;

      if (tentativeG < neighbor->gx)
      {
        neighbor->parent = current;
        neighbor->gx = tentativeG;
        neighbor->hx = heuristic(*neighbor, *goal);

        openList.push(neighbor);
      }
    }
  }
}

float PathFinding::heuristic(Node& A, Node& B)
{
  return static_cast<float>( std::abs(A.x - B.x) + std::abs(A.y - B.y) );
}

void PathFinding::reconstructPath(Node* current, uint64_t id)
{
  std::vector<glm::vec3> path;

  while (current != nullptr)
  {
    path.push_back({current->x * Node::w, current->y * Node::h, 0});
    std::cout << current->x << '\t' << current->y << '\n';

    current = current->parent;
  }
  std::reverse(path.begin(), path.end());
  tasks[id] = path;
}
void PathFinding::animate()
{
  for ( auto& [id, task] :tasks )
  {
    if ( task.empty() ) continue;
    auto pos = task.front();

    float compX = pos.x - (*positionX)[id] + Node::w / 2;
    float compY = pos.y - (*positionY)[id] + Node::h / 2;
    const float length = std::sqrt(float( compX * compX + compY * compY));

    if (length > 10.0f)
    {


      const float directionX = length > 0 ? compX / length : 0.0f;
      const float directionY = length > 0 ? compY / length : 0.0f;


      const float dirX = directionX * (*speedX)[id];
      const float dirY = directionY * (*speedY)[id];


      (*positionX)[id] += dirX * *secondPerFrame;
      (*positionY)[id] += dirY * *secondPerFrame;
  
      (*dirtyPosition)[id] = true;
    }
    else
    {
      task.erase(task.begin());
    }
  }
}