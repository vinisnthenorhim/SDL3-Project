#include "ObjectRender.h"
#include <iostream>

struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
};
struct DefaultMeshObject
{
    static inline std::vector<Vertex> vertex =
    {
        {{-30.0f, -30.0f, 0.0f}, {0.0f, 0.0f}},
        {{ 30.0f, -30.0f, 0.0f}, {1.0f, 0.0f}},
        {{ 30.0f,  30.0f, 0.0f}, {1.0f, 1.0f}},
        {{-30.0f,  30.0f, 0.0f}, {0.0f, 1.0f}}
    };
    static inline std::vector<uint32_t> indices =
    { 0, 1, 2, 0, 2, 3};

};



void ObjectRender::createObjectRender(SDL_Window* window, SDL_GPUDevice* device)
{
  this->device = device;

  Shader vert(device, "vertex.vert", SDL_GPU_SHADERSTAGE_VERTEX, 1, 1);
  Shader frag(device, "fragment.frag", SDL_GPU_SHADERSTAGE_FRAGMENT);

  vertBuff.createBuffer(device, sizeof(Vertex) * DefaultMeshObject::vertex.size(), SDL_GPU_BUFFERUSAGE_VERTEX);
  indexBuff.createBuffer(device, sizeof(uint32_t) * DefaultMeshObject::indices.size(), SDL_GPU_BUFFERUSAGE_INDEX);


  SDL_GPUVertexBufferDescription vertDescription{};

  vertDescription.slot = 0;
  vertDescription.pitch = sizeof(Vertex);
  vertDescription.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
  vertDescription.instance_step_rate = 0;

  SDL_GPUVertexAttribute attributes[2]{};

  attributes[0].location = 0;
  attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT3;
  attributes[0].offset = 0;
  attributes[0].buffer_slot = 0;

  attributes[1].location = 1;
  attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
  attributes[1].offset = sizeof(glm::vec3);
  attributes[1].buffer_slot = 0;

  SDL_GPUVertexInputState state{};

  state.vertex_buffer_descriptions = &vertDescription;
  state.num_vertex_buffers = 1;

  state.vertex_attributes = attributes;
  state.num_vertex_attributes = 2;


  gPipeline.createGraphicPipeline(device, window, vert.shader(), frag.shader(), state);

  std::cout << "[OPENGL]"  << " Buffer created " << '\n'; 
}

void ObjectRender::resourceLoad(const RenderStoreResource resource)
{
  this->type      = resource.type     ;
  this->visible   = resource.visible  ;
  this->positionX = resource.positionX;
  this->positionY = resource.positionY;
  this->positionZ = resource.positionZ;
  this->scaleX    = resource.scaleX   ;
  this->scaleY    = resource.scaleY   ;
  this->scaleZ    = resource.scaleZ   ;
  this->rotationX = resource.rotationX;
  this->rotationY = resource.rotationY;
  this->rotationZ = resource.rotationZ;

  std::cout << "[OPENGL]"  << " Resource Load " << '\n';  

}

void ObjectRender::uploadData( SDL_GPUCopyPass* copy )
{

  vertBuff.startSetData(copy, DefaultMeshObject::vertex);
  indexBuff.startSetData(copy, DefaultMeshObject::indices);


  std::cout << "[SDLGPU]"  << " Buffer data updated " << '\n'; 
}

void ObjectRender::endUpdate()
{
  vertBuff .endSetData();
  indexBuff.endSetData();
}

void ObjectRender::bind(SDL_GPUCommandBuffer* cmd, SDL_GPURenderPass* pass, SDL_GPUBuffer* model)
{
  gPipeline.bind(pass);

  SDL_GPUBufferBinding vertexBinding{};
  vertexBinding.buffer = vertBuff.buffer();
  vertexBinding.offset = 0;

  SDL_BindGPUVertexBuffers( pass, 0, &vertexBinding, 1 );

  SDL_GPUBufferBinding indexBinding{};

  indexBinding.buffer = indexBuff.buffer();
  indexBinding.offset = 0;
  SDL_BindGPUIndexBuffer(pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);

  SDL_GPUBuffer* bufs[] = { model };
  SDL_BindGPUVertexStorageBuffers( pass, 0, bufs, 1);

  SDL_DrawGPUIndexedPrimitives( pass, 6, visible->size(), 0, 0, 0 );

}