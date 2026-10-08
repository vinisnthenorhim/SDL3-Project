#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp> 
#include <glm/gtc/type_ptr.hpp>

class Projection
{
public:
  void createProjection(SDL_Window* window, SDL_GPUDevice* device )
  {
    this->window = window;
    this->device = device;
    updateProjection();           
  }
  void updateProjection()
  {
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    projection = glm::ortho(
        -(float)width/2,
        (float)width/2,
        -(float)height/2,
        (float)height/2
    );             
    

    std::cout << "[SDLGPU] Window is Sizing!" << '\n';
  }
  void uploadProjection(SDL_GPUCommandBuffer* cmd)
  {
    SDL_PushGPUVertexUniformData(cmd, 0, &projection, sizeof(glm::mat4) );
  }
  private:
    SDL_Window* window = nullptr;
    SDL_GPUDevice* device = nullptr;
    glm::mat4 projection = {1};
};
