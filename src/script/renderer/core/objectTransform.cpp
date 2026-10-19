#include "objectTransform.h"
#include <iostream>

#include <fstream>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

void ObjectTransform::createObjectTransform(SDL_GPUDevice* device)
{
  this->device = device;
  FILE* fp = fopen("../src/script/data/path/shader.json", "rb");

  if (!fp) return;

  char buf[16384];
  rapidjson::FileReadStream is(fp, buf, sizeof(buf));

  rapidjson::Document shader_path;
  shader_path.ParseStream(is);
  fclose(fp);

  const char* modelShaderLocation   = shader_path["model"].GetString();

  ComputeShader comp(device, modelShaderLocation);

  SDL_GPUComputePipelineCreateInfo pipelineInfo{};
  pipelineInfo.code = comp.compInfo().code;
  pipelineInfo.code_size = comp.compInfo().code_size;
  pipelineInfo.entrypoint = comp.compInfo().entrypoint;
  pipelineInfo.format = comp.compInfo().format;
  pipelineInfo.num_samplers = 0;
  pipelineInfo.num_readonly_storage_textures = 0;
  pipelineInfo.num_readonly_storage_buffers = 3;
  pipelineInfo.num_readwrite_storage_textures = 0;
  pipelineInfo.num_readwrite_storage_buffers = 4;
  pipelineInfo.num_uniform_buffers = 1;
  pipelineInfo.threadcount_x = 64;
  pipelineInfo.threadcount_y = 1;
  pipelineInfo.threadcount_z = 1;

  computePipeline.createComputePipeline(device, pipelineInfo);
}


void ObjectTransform::loadData(ObjectData objectData, ObjectTransformData objectTransform)
{
    this->positionX = objectData.positionX    ;  this->scalingX = objectData.scalingX    ;  this->rotationX = objectData.rotationX;
    this->positionY = objectData.positionY    ;  this->scalingY = objectData.scalingY    ;  this->rotationY = objectData.rotationY;
    this->positionZ = objectData.positionZ    ;  this->scalingZ = objectData.scalingZ    ;  this->rotationZ = objectData.rotationZ; 

    this->dirtyVelocity = objectTransform.dirtyVelocity;  this->dirtyScale  = objectTransform.dirtyScale;  this->dirtyRotate = objectTransform.dirtyRotate;
    this->velocityX     = objectTransform.velocityX    ;  this->scaleX      = objectTransform.scaleX    ;  this->rotateX     = objectTransform.rotateX    ;
    this->velocityY     = objectTransform.velocityY    ;  this->scaleY      = objectTransform.scaleY    ;  this->rotateY     = objectTransform.rotateY    ;
    this->velocityZ     = objectTransform.velocityZ    ;  this->scaleZ      = objectTransform.scaleZ    ;  this->rotateZ     = objectTransform.rotateZ    ;

    this->speedX = objectData.speedX; this->speedY = objectData.speedY; this->visible = objectData.visible; this->deltaTime = objectTransform.secondPerFrame;
}

void ObjectTransform::updateDataSize( SDL_GPUCopyPass* copypass)
{
  Uint32 currentCount = visible->size();
  if(currentCount <= entityCount || currentCount <= 0) return;

  Uint32 offset = entityCount * sizeof(glm::vec4);
  Uint32 newCount = currentCount - entityCount;
  Uint32 newBytes = newCount * sizeof(glm::vec4); 

  position.resize(currentCount);                              
  scaling .resize(currentCount);
  rotation.resize(currentCount);

  std::vector<glm::vec4> tempPosData{};
  std::vector<glm::vec4> tempSclData{};
  std::vector<glm::vec4> tempRotData{};

  for( Uint32 a = entityCount; a < currentCount; a++)
  {
    position[a].x = (*positionX)[a];
    position[a].y = (*positionY)[a];
    position[a].z = (*positionZ)[a];

    scaling[a].x  = (*scalingX)[a];
    scaling[a].y  = (*scalingY)[a];
    scaling[a].z  = (*scalingZ)[a];

    rotation[a].x = (*rotationX)[a];
    rotation[a].y = (*rotationY)[a];
    rotation[a].z = (*rotationZ)[a];

    tempPosData.emplace_back((*positionX)[a], (*positionY)[a], (*positionZ)[a], 1.0f);
    tempSclData.emplace_back((*scalingX)[a] , (*scalingY)[a] , (*scalingZ)[a] , 1.0f);
    tempRotData.emplace_back((*rotationX)[a], (*rotationY)[a], (*rotationZ)[a], 1.0f);
  }

  if (!positionBuffer.buffer() ) positionBuffer.createBuffer(device, sizeof(glm::vec4) * position.size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {positionBuffer.updateBufferSize( copypass, sizeof(glm::vec4) * position.size(), true );}
  if (!scalingBuffer.buffer()  ) scalingBuffer .createBuffer(device, sizeof(glm::vec4) * scaling.size(),  SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {scalingBuffer .updateBufferSize( copypass, sizeof(glm::vec4) * scaling.size() , true );}
  if (!rotationBuffer.buffer() ) rotationBuffer.createBuffer(device, sizeof(glm::vec4) * rotation.size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {rotationBuffer.updateBufferSize( copypass, sizeof(glm::vec4) * rotation.size(), true );}
  if (!modelBuffer.buffer()    ) modelBuffer   .createBuffer(device, sizeof(glm::mat4) * visible->size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {modelBuffer   .updateBufferSize( copypass, sizeof(glm::mat4) * rotation.size(), true );}

  positionTransfer.createTransferBuffer(device, newBytes);
  scalingTransfer .createTransferBuffer(device, newBytes);
  rotationTransfer.createTransferBuffer(device, newBytes);
  if(!tempPosData.empty())
  {
    uint8_t* positionMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, positionTransfer.buffer(), false);
    SDL_memcpy(positionMapped, tempPosData.data(), newBytes);
    SDL_UnmapGPUTransferBuffer(device, positionTransfer.buffer());

    SDL_GPUTransferBufferLocation possrc = {};
    SDL_GPUBufferRegion           posdst = {};

    possrc.transfer_buffer = positionTransfer.buffer();
    possrc.offset = 0;
    posdst.buffer = positionBuffer.buffer();
    posdst.offset = offset;
    posdst.size   = newBytes;
    SDL_UploadToGPUBuffer(copypass, &possrc, &posdst, false);
  }
  if(!tempSclData.empty())
  {
    uint8_t* scalingMapped  = (uint8_t*)SDL_MapGPUTransferBuffer(device,  scalingTransfer.buffer(), false);
    SDL_memcpy(scalingMapped  , tempSclData.data(), newBytes);
    SDL_UnmapGPUTransferBuffer(device, scalingTransfer.buffer());

    SDL_GPUTransferBufferLocation sclsrc = {};
    SDL_GPUBufferRegion           scldst = {};

    sclsrc.transfer_buffer = scalingTransfer.buffer();
    sclsrc.offset = 0;
    scldst.buffer = scalingBuffer.buffer();
    scldst.offset = offset;
    scldst.size   = newBytes;
    SDL_UploadToGPUBuffer(copypass, &sclsrc, &scldst, false);
  }
  if(!tempRotData.empty())
  {
    uint8_t* rotationMapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, rotationTransfer.buffer(), false);
    SDL_memcpy(rotationMapped , tempRotData.data(), newBytes);
    SDL_UnmapGPUTransferBuffer(device, rotationTransfer.buffer());

    SDL_GPUTransferBufferLocation rotsrc = {};
    SDL_GPUBufferRegion           rotdst = {};

    rotsrc.transfer_buffer = rotationTransfer.buffer();
    rotsrc.offset = 0;
    rotdst.buffer = rotationBuffer.buffer();
    rotdst.offset = offset;
    rotdst.size   = newBytes;

    SDL_UploadToGPUBuffer(copypass, &rotsrc, &rotdst, false);
  }
}



void ObjectTransform::updateTransform( SDL_GPUCopyPass* copypass)
{
  Uint32 currentCount = visible->size();
  if( currentCount <= 0 ) return;

  Uint32 bufferSize = 3 * visible->size() * sizeof(float);
  Uint32 offset = sizeof(float) * visible->size();

  if (!velocityBuffer.buffer() )  velocityBuffer.createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {velocityBuffer.updateBufferSize(copypass, bufferSize, false);}
  if (!scaleBuffer.buffer()    )  scaleBuffer   .createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {scaleBuffer   .updateBufferSize(copypass, bufferSize, false);}
  if (!rotateBuffer.buffer()   )  rotateBuffer  .createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {rotateBuffer  .updateBufferSize(copypass, bufferSize, false);}

  std::vector<int> tempVelDirty {};
  std::vector<int> tempSclDirty {};
  std::vector<int> tempRotDirty {};

  for ( int a = 0; a < dirtyVelocity->size(); a++ ) 
  {
    if((*dirtyVelocity)[a] == true) tempVelDirty.push_back(a);
  }
  for ( int a = 0; a < dirtyScale->size(); a++ ) 
  {
    if((*dirtyScale)[a] == true) tempSclDirty.push_back(a);  
  }  
  for ( int a = 0; a < dirtyRotate->size(); a++ ) 
  {
    if((*dirtyRotate)[a] == true) tempRotDirty.push_back(a); 
  }
  Uint32 updateVelSize = tempVelDirty.size() * sizeof(float);
  Uint32 updateSclSize = tempSclDirty.size() * sizeof(float);
  Uint32 updateRotSize = tempRotDirty.size() * sizeof(float);

  if (!tempVelDirty.empty())
  {
    veloTrans.createTransferBuffer(device, 3 * updateVelSize);
    int slot = 0;
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, veloTrans.buffer(), false);
    if (!mapped) return;
    for ( int a : tempVelDirty )
    {
      SDL_memcpy(mapped                     + slot * sizeof(float) , &(*velocityX)[a], sizeof(float));
      SDL_memcpy(mapped +     updateVelSize + slot * sizeof(float) , &(*velocityY)[a], sizeof(float));
      SDL_memcpy(mapped + 2 * updateVelSize + slot * sizeof(float) , &(*velocityZ)[a], sizeof(float));
      slot++;
    }
    SDL_UnmapGPUTransferBuffer(device, veloTrans.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = veloTrans.buffer();

    SDL_GPUBufferRegion dst = {};
    dst.buffer = velocityBuffer.buffer();
    dst.size   = sizeof(float);
    slot = 0;

    for ( int a : tempVelDirty )
    {
      src.offset = slot * sizeof(float)                    ; dst.offset = a * sizeof(float)             ; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) +     updateVelSize; dst.offset = a * sizeof(float) +     offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) + 2 * updateVelSize; dst.offset = a * sizeof(float) + 2 * offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      slot++;
    }
  }

  if (!tempSclDirty.empty())
  {
    scalTrans.createTransferBuffer(device, 3 * updateSclSize);
    int slot = 0;
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, scalTrans.buffer(), false);
    if (!mapped) return;
    for ( int a : tempSclDirty )
    {
      SDL_memcpy(mapped                     + slot * sizeof(float) , &(*scaleX)[a], sizeof(float));
      SDL_memcpy(mapped +     updateSclSize + slot * sizeof(float) , &(*scaleY)[a], sizeof(float));
      SDL_memcpy(mapped + 2 * updateSclSize + slot * sizeof(float) , &(*scaleZ)[a], sizeof(float));
      slot++;
    }
    SDL_UnmapGPUTransferBuffer(device, scalTrans.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = scalTrans.buffer();

    SDL_GPUBufferRegion dst = {};
    dst.buffer = scaleBuffer.buffer();
    dst.size   = sizeof(float);
    slot = 0;
    for ( int a : tempSclDirty )
    {
      src.offset = slot * sizeof(float)                    ; dst.offset = a * sizeof(float)             ; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) +     updateSclSize; dst.offset = a * sizeof(float) +     offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) + 2 * updateSclSize; dst.offset = a * sizeof(float) + 2 * offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      slot++;
    }
  }

  if (!tempRotDirty.empty())
  {
    rotaTrans.createTransferBuffer(device, 3 * updateRotSize);
    int slot = 0;
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, rotaTrans.buffer(), false);
    if (!mapped) return;
    for ( int a : tempRotDirty )
    {
      SDL_memcpy(mapped                     + slot * sizeof(float) , &(*rotateX)[a], sizeof(float));
      SDL_memcpy(mapped + updateRotSize     + slot * sizeof(float) , &(*rotateY)[a], sizeof(float));
      SDL_memcpy(mapped + 2 * updateRotSize + slot * sizeof(float) , &(*rotateZ)[a], sizeof(float));
      slot++;
    }
    SDL_UnmapGPUTransferBuffer(device, rotaTrans.buffer());
    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = rotaTrans.buffer();

    SDL_GPUBufferRegion dst = {};
    dst.buffer = rotateBuffer.buffer();
    dst.size   = sizeof(float);
    slot = 0;
    for ( int a : tempRotDirty )
    {
      src.offset = slot * sizeof(float)                    ; dst.offset = a * sizeof(float)             ; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) +     updateRotSize; dst.offset = a * sizeof(float) +     offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      src.offset = slot * sizeof(float) + 2 * updateRotSize; dst.offset = a * sizeof(float) + 2 * offset; SDL_UploadToGPUBuffer(copypass, &src, &dst, false);
      slot++;
    }
  }
}

struct UniformData
{
  float deltaTime;
  Uint32 entityCount;
};

void ObjectTransform::bind(SDL_GPUCommandBuffer* cmd)
{
  entityCount = visible->size();
  if (!computePipeline.pipeline() || entityCount == 0) return;

  Uint32 groupCount = (entityCount + 64 - 1) / 64;
  UniformData uniformData = {*deltaTime, entityCount};

  SDL_GPUStorageBufferReadWriteBinding rw[4] = {};
  rw[0].buffer = modelBuffer.buffer();
  rw[0].cycle  = false;

  rw[1].buffer = positionBuffer.buffer();
  rw[1].cycle  = false;

  rw[2].buffer = scalingBuffer.buffer();
  rw[2].cycle  = false;

  rw[3].buffer = rotationBuffer.buffer();
  rw[3].cycle  = false;

  // SDL_Log("11");
  SDL_GPUComputePass* computePass = SDL_BeginGPUComputePass(cmd, nullptr, 0, rw, 4);
  // SDL_Log("12");

  SDL_BindGPUComputePipeline( computePass, computePipeline.pipeline() );
  // SDL_Log("13");

  SDL_GPUBuffer* ro[] = 
  {
    velocityBuffer.buffer(),
    scaleBuffer.buffer(),
    rotateBuffer.buffer()
  };
  // SDL_Log("14");
  SDL_BindGPUComputeStorageBuffers( computePass, 0, ro, 3);

  // SDL_Log("15");
  SDL_PushGPUComputeUniformData(cmd, 0, &uniformData, sizeof(uniformData));

  // SDL_Log("16");
  SDL_DispatchGPUCompute(computePass, groupCount, 1, 1);

  // SDL_Log("17");
  SDL_EndGPUComputePass(computePass);
}

void ObjectTransform::release()
{
  velocityBuffer.releaseOldBuffer();
  scaleBuffer   .releaseOldBuffer();
  rotateBuffer  .releaseOldBuffer(); 
  modelBuffer   .releaseOldBuffer();
  positionBuffer.releaseOldBuffer();
  scalingBuffer .releaseOldBuffer();
  rotationBuffer.releaseOldBuffer();

  positionTransfer.release();
  scalingTransfer .release();
  rotationTransfer.release();
  veloTrans       .release();
  scalTrans       .release();
  rotaTrans       .release();
}