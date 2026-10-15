#include "camera.h"

void CameraHandle::createCameraHandle(SDL_GPUDevice* device)
{
  this->device = device;

  ComputeShader comp(device, "camera.comp");

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
  this->entityId = cameraPosition.entityID;
  this->deltaTime = cameraPosition.deltaTime;
  this->dirty = cameraPosition.dirty;

  this->velocityX = cameraPosition.velocityX  ;
  this->velocityY = cameraPosition.velocityY  ;
  this->velocityZ = cameraPosition.velocityZ  ;

  camPositionX = cameraPosition.positionX;
  camPositionY = cameraPosition.positionY;
  camPositionZ = cameraPosition.positionZ;
}
void CameraHandle::createCamera( SDL_GPUCopyPass* copypass)
{
  if (created) return;
  created = true;

  cameraVelocity.createBuffer(device, 3 * sizeof(float), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  cameraPosition.createBuffer(device, sizeof(glm::vec4), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  cameraMatrice .createBuffer(device, sizeof(glm::mat4), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);

  cameraVelocity.updateBufferSize(copypass, 3 * sizeof(float), false);

  glm::vec4 pos;
  pos.x = (*positionX)[*entityId];
  pos.y = (*positionZ)[*entityId];
  pos.z = (*positionZ)[*entityId];
  pos.w = 1;
  positionTransfer.createTransferBuffer(device, sizeof(glm::vec4));
  void* positionMapped = (void*)SDL_MapGPUTransferBuffer(device, positionTransfer.buffer(), false);
  SDL_memcpy(positionMapped, &pos, sizeof(glm::vec4));
  SDL_UnmapGPUTransferBuffer(device, positionTransfer.buffer());

  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = positionTransfer.buffer();
  possrc.offset = 0;
  posdst.buffer = cameraPosition.buffer();
  posdst.offset = 0;
  posdst.size   = sizeof(glm::vec4);

  SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);

}

void CameraHandle::updateCameraData( SDL_GPUCopyPass* copypass)
{
  if ( *follow != 0u ) return;
  if ( *dirty == false) return;

  posUpdateTransfer.createTransferBuffer(device, 3 * sizeof(float));

  uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, posUpdateTransfer.buffer(), false);
  if(!positionMapped)
  {
    return;
  }
  SDL_memcpy(positionMapped + 0 * sizeof(float), velocityX, sizeof(float));
  SDL_memcpy(positionMapped + 1 * sizeof(float), velocityY, sizeof(float));
  SDL_memcpy(positionMapped + 2 * sizeof(float), velocityZ, sizeof(float));
  SDL_UnmapGPUTransferBuffer(device, posUpdateTransfer.buffer());

  SDL_GPUTransferBufferLocation possrc = {};
  SDL_GPUBufferRegion           posdst = {};

  possrc.transfer_buffer = posUpdateTransfer.buffer();
  possrc.offset = 0;
  
  posdst.buffer = cameraVelocity.buffer();
  posdst.offset = 0;
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
