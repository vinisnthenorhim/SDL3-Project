#include "ObjectRender.h"
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>
struct Vertex
{
    glm::vec3 position;
    glm::vec2 uv;
};
struct DefaultMeshObject
{
    static inline std::vector<Vertex> vertex =
    {
        {{-60.0f, -60.0f, 0.0f}, {0.0f, 1.0f}},
        {{ 60.0f, -60.0f, 0.0f}, {1.0f, 1.0f}},
        {{ 60.0f,  60.0f, 0.0f}, {1.0f, 0.0f}},
        {{-60.0f,  60.0f, 0.0f}, {0.0f, 0.0f}}
    };
    static inline std::vector<uint32_t> indices =
    { 0, 1, 2, 0, 2, 3};

};

void ObjectRender::createObjectRender(SDL_Window* window, SDL_GPUDevice* device)
{
  this->device = device;

  BW::File fp = {"../src/data/json/path/shader.json", "rb"};


  char buf[16384];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document shader_path;
  shader_path.ParseStream(is);

  const char* vertexShaderLocation   = shader_path["vertex"].GetString();
  const char* fragmentShaderLocation = shader_path["fragment"].GetString();

  SDL_GPUShaderCreateInfo vertInfo{};
  vertInfo.stage      = SDL_GPU_SHADERSTAGE_VERTEX;
  vertInfo.num_uniform_buffers = 1;
  vertInfo.num_storage_buffers = 2;

  SDL_GPUShaderCreateInfo fragInfo{};
  fragInfo.stage      =  SDL_GPU_SHADERSTAGE_FRAGMENT;
  fragInfo.num_samplers = 1;


  Shader vert(device, vertexShaderLocation, vertInfo);
  Shader frag(device, fragmentShaderLocation, fragInfo);

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

  SDL_GPUColorTargetDescription colorTarget = {};

  colorTarget.format = SDL_GetGPUSwapchainTextureFormat(device, window);
  colorTarget.blend_state.enable_blend          = true;
  colorTarget.blend_state.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
  colorTarget.blend_state.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
  colorTarget.blend_state.color_blend_op        = SDL_GPU_BLENDOP_ADD;
  colorTarget.blend_state.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
  colorTarget.blend_state.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
  colorTarget.blend_state.alpha_blend_op        = SDL_GPU_BLENDOP_ADD;

  SDL_GPUGraphicsPipelineCreateInfo pipelineInfo {};

  pipelineInfo.primitive_type  = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
  pipelineInfo.target_info.color_target_descriptions = &colorTarget;
  pipelineInfo.target_info.num_color_targets         = 1;
  pipelineInfo.vertex_shader   = vert.shader();
  pipelineInfo.fragment_shader = frag.shader();
  pipelineInfo.vertex_input_state = state;

  gPipeline.createGraphicPipeline(device, pipelineInfo);

}

void ObjectRender::resourceLoad(ObjectData objectData)
{
  this->visible   = objectData.visible  ;
}

void ObjectRender::uploadData( SDL_GPUCopyPass* copy )
{
  if(upload) return;
  vertBuff.startSetData(copy, DefaultMeshObject::vertex);
  indexBuff.startSetData(copy, DefaultMeshObject::indices);
}

void ObjectRender::endUpdate()
{
  if(upload) return;
  upload = true;
  vertBuff .endSetData();
  indexBuff.endSetData();
}

void ObjectRender::bind( SDL_GPURenderPass* pass, SDL_GPUBuffer* model, SDL_GPUBuffer* camera, SDL_GPUTextureSamplerBinding textureBind)
{
  if (visible->size() <= 0) return;
  gPipeline.bind(pass);
  // SDL_Log("7.1");

  SDL_GPUBufferBinding vertexBinding{};
  vertexBinding.buffer = vertBuff.buffer();
  vertexBinding.offset = 0;
  // SDL_Log("7.2");

  SDL_BindGPUVertexBuffers( pass, 0, &vertexBinding, 1 );
  // SDL_Log("7.3");

  SDL_GPUBufferBinding indexBinding{};
  // SDL_Log("7.4");

  indexBinding.buffer = indexBuff.buffer();
  indexBinding.offset = 0;
  SDL_BindGPUIndexBuffer(pass, &indexBinding, SDL_GPU_INDEXELEMENTSIZE_32BIT);
  // SDL_Log("7.5");

  SDL_GPUBuffer* bufs[] = { model, camera };
  SDL_BindGPUVertexStorageBuffers( pass, 0, bufs, 2);
  // SDL_Log("7.6");
  // SDL_Log("tex=%p samp=%p", (void*)textureBind.texture, (void*)textureBind.sampler);

  SDL_BindGPUFragmentSamplers(pass, 0, &textureBind, 1);
  // SDL_Log("7.7");

  SDL_DrawGPUIndexedPrimitives( pass, 6, visible->size(), 0, 0, 0 );
}