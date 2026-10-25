#include "camera.h"
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

void CameraHandle::createCameraHandle(SDL_GPUDevice* device)
{
  this->device = device;
  BW::File fp = {"../src/data/json/path/shader.json", "rb"};


  char buf[16384];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document shader_path;
  shader_path.ParseStream(is);

  const char* shaderLocation = shader_path["camera"].GetString();

  ComputeShader comp(device, shaderLocation);

  SDL_GPUComputePipelineCreateInfo pipelineInfo{};
  pipelineInfo.code = comp.compInfo().code;
  pipelineInfo.code_size = comp.compInfo().code_size;
  pipelineInfo.entrypoint = comp.compInfo().entrypoint;
  pipelineInfo.format = comp.compInfo().format;
  pipelineInfo.num_samplers = 0;
  pipelineInfo.num_readonly_storage_textures = 0;
  pipelineInfo.num_readonly_storage_buffers = 2;
  pipelineInfo.num_readwrite_storage_textures = 0;
  pipelineInfo.num_readwrite_storage_buffers = 2;
  pipelineInfo.num_uniform_buffers = 1;
  pipelineInfo.threadcount_x = 1;
  pipelineInfo.threadcount_y = 1;
  pipelineInfo.threadcount_z = 1;

  cameraPipeline.createComputePipeline(device, pipelineInfo);
}

void CameraHandle::loadData(ObjectData objectData, Position cameraPosition)
{
  this->positionX = objectData.positionX    ;
  this->positionY = objectData.positionY    ;
  this->positionZ = objectData.positionZ    ;

  this->follow   = cameraPosition.follow  ;
  this->entityId = objectData.playerId;
  this->deltaTime = cameraPosition.deltaTime;
  this->dirty = cameraPosition.dirty;

  this->updateDataX = cameraPosition.updateDataX  ;
  this->updateDataY = cameraPosition.updateDataY  ;
  this->updateDataZ = cameraPosition.updateDataZ  ;

  camPositionX = cameraPosition.positionX;
  camPositionY = cameraPosition.positionY;
  camPositionZ = cameraPosition.positionZ;
}
void CameraHandle::createCamera( SDL_GPUCopyPass* copypass)
{
  if ( positionX->size() <= 0) return;
  if ( created ) return;
  created = true;

  cameraVelocity.createBuffer(device, 12 * sizeof(float), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  cameraPosition.createBuffer(device, 4 * sizeof(glm::vec4), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  cameraMatrice .createBuffer(device,  sizeof(glm::mat4), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);

  cameraVelocity.updateBufferSize(copypass, 12 * sizeof(float), false);

  std::array<glm::vec4, 4> pos;
  pos[0].x = (*positionX)[*entityId];
  pos[0].y = (*positionY)[*entityId];
  pos[0].z = (*positionZ)[*entityId];
  pos[0].w = 1;

  for (int a = 1; a < 4 ; a++)
  {
    pos[a].x = (*camPositionX)[a];
    pos[a].y = (*camPositionY)[a];
    pos[a].z = (*camPositionZ)[a];
    pos[a].w = 1;
  }
  

  positionTransfer.createTransferBuffer(device, 4 * sizeof(glm::vec4));
  uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, positionTransfer.buffer(), false);
  SDL_memcpy(positionMapped, &pos, 4 * sizeof(glm::vec4));
  SDL_UnmapGPUTransferBuffer(device, positionTransfer.buffer());
  SDL_GPUTransferBufferLocation possrc2 = {};
  SDL_GPUBufferRegion           posdst2 = {};


  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = positionTransfer.buffer();
  possrc.offset = 0;
  posdst.buffer = cameraPosition.buffer();
  posdst.offset = 0;
  posdst.size   = 4 * sizeof(glm::vec4);

  SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);
}

void CameraHandle::updateCameraData( SDL_GPUCopyPass* copypass)
{
  if ( positionX->size() <= 0) return;
  if ( *dirty == false) return;

  if ( *follow != 0u ) 
  {
    followEntity(copypass);
    return;
  }

  posUpdateTransfer.createTransferBuffer(device, 12 * sizeof(float));

  float veloX = (*updateDataX)[0];
  float veloY = (*updateDataY)[0];
  float veloZ = (*updateDataZ)[0];
  float scaleX = (*updateDataX)[1];
  float scaleY = (*updateDataY)[1];
  float scaleZ = (*updateDataZ)[1];
  float rotateX = (*updateDataX)[2];
  float rotateY = (*updateDataY)[2];
  float rotateZ = (*updateDataZ)[2];
  float winPosX = (*updateDataX)[3];
  float winPosY = (*updateDataY)[3];
  float winPosZ = (*updateDataZ)[3];


  uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, posUpdateTransfer.buffer(), false);
  if(!positionMapped)
  {
    return;
  }
  SDL_memcpy(positionMapped + 0 * sizeof(float),  &veloX,   sizeof(float));
  SDL_memcpy(positionMapped + 1 * sizeof(float),  &veloY,   sizeof(float));
  SDL_memcpy(positionMapped + 2 * sizeof(float),  &veloZ,   sizeof(float));
  SDL_memcpy(positionMapped + 3 * sizeof(float),  &scaleX,  sizeof(float));
  SDL_memcpy(positionMapped + 4 * sizeof(float),  &scaleY,  sizeof(float));
  SDL_memcpy(positionMapped + 5 * sizeof(float),  &scaleZ,  sizeof(float));
  SDL_memcpy(positionMapped + 6 * sizeof(float),  &rotateX, sizeof(float));
  SDL_memcpy(positionMapped + 7 * sizeof(float),  &rotateY, sizeof(float));
  SDL_memcpy(positionMapped + 8 * sizeof(float),  &rotateZ, sizeof(float));
  SDL_memcpy(positionMapped + 9  * sizeof(float), &winPosX, sizeof(float));
  SDL_memcpy(positionMapped + 10 * sizeof(float), &winPosY, sizeof(float));
  SDL_memcpy(positionMapped + 11 * sizeof(float), &winPosZ, sizeof(float));
  SDL_UnmapGPUTransferBuffer(device, posUpdateTransfer.buffer());

  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = posUpdateTransfer.buffer();
  possrc.offset = 0;
  
  posdst.buffer = cameraVelocity.buffer();
  posdst.offset = 0;
  posdst.size   = 12 * sizeof(float);

  SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);

}
void CameraHandle::followEntity(SDL_GPUCopyPass* copypass)
{
  float winPosX = (*updateDataX)[3];
  float winPosY = (*updateDataY)[3];
  float winPosZ = (*updateDataZ)[3];
  posUpdateTransfer.createTransferBuffer(device, 3 * sizeof(float));
 uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, posUpdateTransfer.buffer(), false);
  if(!positionMapped)
  {
    return;
  }
  SDL_memcpy(positionMapped + 0 * sizeof(float), &winPosX, sizeof(float));
  SDL_memcpy(positionMapped + 1 * sizeof(float), &winPosY, sizeof(float));
  SDL_memcpy(positionMapped + 2 * sizeof(float), &winPosZ, sizeof(float));
  SDL_UnmapGPUTransferBuffer(device, posUpdateTransfer.buffer());

  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = posUpdateTransfer.buffer();
  possrc.offset = 0;
  
  posdst.buffer = cameraVelocity.buffer();
  posdst.offset = 9 * sizeof(float);
  posdst.size   = 3 * sizeof(float);

  SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);

}
struct UniformData
{
  float deltaTime;
  Uint32 follow;
  Uint32 entityId;
};

void CameraHandle::bind( SDL_GPUCommandBuffer* cmd, SDL_GPUBuffer* position)
{
  if ( positionX->size() <= 0) return;
  UniformData uniformData = {*deltaTime, *follow, *entityId};

  SDL_GPUStorageBufferReadWriteBinding rw[2] = {};
  rw[0].buffer = cameraPosition.buffer();
  rw[0].cycle  = false;

  rw[1].buffer = cameraMatrice.buffer();
  rw[1].cycle  = false;

  SDL_GPUComputePass* computePass = SDL_BeginGPUComputePass(cmd, nullptr, 0, rw, 2);

  SDL_BindGPUComputePipeline( computePass, cameraPipeline.pipeline() );

  SDL_GPUBuffer* ro[] = 
  {
    cameraVelocity.buffer(),
    position
  };
  SDL_BindGPUComputeStorageBuffers( computePass, 0, ro, 2);

  SDL_PushGPUComputeUniformData(cmd, 0, &uniformData, sizeof(uniformData));

  SDL_DispatchGPUCompute(computePass, 1, 1, 1);

  SDL_EndGPUComputePass(computePass);
}

void CameraHandle::releaseOld()
{
  positionTransfer.release();
  posUpdateTransfer.release();
}

void CameraHandle::bindG(SDL_GPURenderPass* pass, uint32_t slot)
{
  if (positionX->size() <= 0) return;
  SDL_GPUBuffer* bufs[] = { cameraMatrice.buffer()};

  SDL_BindGPUVertexStorageBuffers( pass, slot, bufs, 1);
}