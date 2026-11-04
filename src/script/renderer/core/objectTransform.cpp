#include "objectTransform.h"
#include <iostream>
#include <loader/BW.h>
#include <rapidjson/document.h>
#include <rapidjson/filereadstream.h>

void ObjectTransform::createObjectTransform(SDL_GPUDevice* device)
{
  this->device = device;
  BW::File fp = {"../src/data/json/path/shader.json", "rb"};


  char buf[16384];
  rapidjson::FileReadStream is(fp.fp, buf, sizeof(buf));

  rapidjson::Document shader_path;
  shader_path.ParseStream(is);

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


void ObjectTransform::loadData( ObjectDataRO objectDataRO, DirtyData dirtyData )
{
    this->positionX = objectDataRO.positionX    ;  this->scalingX = objectDataRO.scalingX    ;  this->rotationX = objectDataRO.rotationX;
    this->positionY = objectDataRO.positionY    ;  this->scalingY = objectDataRO.scalingY    ;  this->rotationY = objectDataRO.rotationY;
    this->positionZ = objectDataRO.positionZ    ;  this->scalingZ = objectDataRO.scalingZ    ;  this->rotationZ = objectDataRO.rotationZ; 

    this->velocityX     = objectDataRO.velocityX    ;  this->scaleX      = objectDataRO.scaleX    ;  this->rotateX     = objectDataRO.rotateX    ;
    this->velocityY     = objectDataRO.velocityY    ;  this->scaleY      = objectDataRO.scaleY    ;  this->rotateY     = objectDataRO.rotateY    ;
    this->velocityZ     = objectDataRO.velocityZ    ;  this->scaleZ      = objectDataRO.scaleZ    ;  this->rotateZ     = objectDataRO.rotateZ    ;

    this->dirtyPosition  = dirtyData.dirtyPosition;
    this->dirtyScaling   = dirtyData.dirtyScaling ;
    this->dirtyRotation  = dirtyData.dirtyRotation;

    this->dirtyVelocity = dirtyData.dirtyVelocity   ;  
    this->dirtyScale    = dirtyData.dirtyScale      ;  
    this->dirtyRotate   = dirtyData.dirtyRotate     ;


    this->speedX = objectDataRO.speedX; 
    this->speedY = objectDataRO.speedY; 
    this->exist = objectDataRO.exist; 
    this->secondPerFrame = objectDataRO.secondPerFrame;
}

void ObjectTransform::updateDataSize( SDL_GPUCopyPass* copypass)
{
  Uint32 currentCount = exist->size();

  size_t size = currentCount * 3 * sizeof(float); 
  Uint32 offset = sizeof(float) * exist->size();


  if (!positionBuffer.buffer() ) positionBuffer.createBuffer(device, size, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {positionBuffer.updateBufferSize( copypass, size , false );}
  if (!scalingBuffer.buffer()  ) scalingBuffer .createBuffer(device, size,  SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {scalingBuffer .updateBufferSize( copypass, size , false );}
  if (!rotationBuffer.buffer() ) rotationBuffer.createBuffer(device, size, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {rotationBuffer.updateBufferSize( copypass, size , false );}
  if (!modelBuffer.buffer()    ) modelBuffer   .createBuffer(device, sizeof(glm::mat4) * exist->size(), SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ | SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_WRITE);
  else {modelBuffer   .updateBufferSize( copypass, sizeof(glm::mat4) * exist->size(), true );}

  bool posDirty = false;
  bool sclDirty = false;
  bool rotDirty = false;


  for ( int a = 0; a < dirtyPosition->size(); a++ ) 
  {
    if((*dirtyPosition)[a] == true) 
    {
      posDirty = true;
            for ( int a = 0; a < dirtyPosition->size(); a++ ) 
        (*dirtyPosition)[a] == false;
      break;
    }
  }
  for ( int a = 0; a < dirtyScaling->size(); a++ ) 
  {
    if((*dirtyScaling)[a] == true) 
    {
      sclDirty = true;
      for ( int a = 0; a < dirtyScaling->size(); a++ ) 
        (*dirtyScaling)[a] == false;
      break;
    }
  }  
  for ( int a = 0; a < dirtyRotation->size(); a++ ) 
  {
    if((*dirtyRotation)[a] == true) 
    {
      rotDirty = true;
      for ( int a = 0; a < dirtyRotation->size(); a++ ) 
        (*dirtyRotation)[a] == false;
      break;
    }
  }

  if (posDirty)
  {
    positionTransfer.createTransferBuffer(device, 3 * sizeof(float) * exist->size());
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, positionTransfer.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , positionX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , positionY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , positionZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, positionTransfer.buffer());


    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = positionTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = positionBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 
    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }

  if (sclDirty)
  {
    scalingTransfer.createTransferBuffer(device, 3 * sizeof(float) * exist->size());
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, scalingTransfer.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , scalingX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , scalingY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , scalingZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, scalingTransfer.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = scalingTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = scalingBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 

    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }

  if (rotDirty)
  {
    rotationTransfer.createTransferBuffer(device,   3 * sizeof(float) * exist->size());

    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, rotationTransfer.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , rotationX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , rotationY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , rotationZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, rotationTransfer.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = rotationTransfer.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = rotationBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 

    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }
}


void ObjectTransform::updateTransform( SDL_GPUCopyPass* copypass)
{
  Uint32 currentCount = exist->size();
  if( currentCount <= 0 ) return;

  Uint32 bufferSize = 3 * exist->size() * sizeof(float);
  Uint32 offset = sizeof(float) * exist->size();

  if (!velocityBuffer.buffer() )  velocityBuffer.createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {velocityBuffer.updateBufferSize(copypass, bufferSize, false);}
  if (!scaleBuffer.buffer()    )  scaleBuffer   .createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {scaleBuffer   .updateBufferSize(copypass, bufferSize, false);}
  if (!rotateBuffer.buffer()   )  rotateBuffer  .createBuffer(device, bufferSize, SDL_GPU_BUFFERUSAGE_COMPUTE_STORAGE_READ);
  else {rotateBuffer  .updateBufferSize(copypass, bufferSize, false);}

  bool velDirty = false;
  bool sclDirty = false;
  bool rotDirty = false;


  for ( int a = 0; a < dirtyVelocity->size(); a++ ) 
  {
    if((*dirtyVelocity)[a] == true) 
    {
      velDirty = true;
      for ( int a = 0; a < dirtyVelocity->size(); a++ ) 
        (*dirtyVelocity)[a] == false;
      break;
    }
  }
  for ( int a = 0; a < dirtyScale->size(); a++ ) 
  {
    if((*dirtyScale)[a] == true) 
    {
      sclDirty = true;
      for ( int a = 0; a < dirtyScale->size(); a++ ) 
        (*dirtyScale)[a] == false;
      break;
    }
  }  
  for ( int a = 0; a < dirtyRotate->size(); a++ ) 
  {
    if((*dirtyRotate)[a] == true) 
    {
      rotDirty = true;
      for ( int a = 0; a < dirtyRotate->size(); a++ ) 
        (*dirtyRotate)[a] == false;
      break;
    }
  }

  if (velDirty)
  {
    veloTrans.createTransferBuffer(device, 3 * sizeof(float) * exist->size());
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, veloTrans.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , velocityX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , velocityY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , velocityZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, veloTrans.buffer());


    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = veloTrans.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = velocityBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 
    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }

  if (sclDirty)
  {
    scalTrans.createTransferBuffer(device, 3 * sizeof(float) * exist->size());
    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, scalTrans.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , scaleX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , scaleY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , scaleZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, scalTrans.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = scalTrans.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = scaleBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 

    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }

  if (rotDirty)
  {
    rotaTrans.createTransferBuffer(device,   3 * sizeof(float) * exist->size());

    uint8_t* mapped = (uint8_t*)SDL_MapGPUTransferBuffer(device, rotaTrans.buffer(), false);
    if (!mapped) return;
    SDL_memcpy(mapped + 0 * offset , rotateX->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 1 * offset , rotateY->data(), exist->size() * sizeof(float));
    SDL_memcpy(mapped + 2 * offset , rotateZ->data(), exist->size() * sizeof(float));
    SDL_UnmapGPUTransferBuffer(device, rotaTrans.buffer());

    SDL_GPUTransferBufferLocation src = {};
    src.transfer_buffer = rotaTrans.buffer();
    src.offset = 0                ;

    SDL_GPUBufferRegion dst = {};
    dst.buffer = rotateBuffer.buffer();
    dst.size   = 3 * sizeof(float) * exist->size();
    dst.offset = 0         ; 

    SDL_UploadToGPUBuffer(copypass, &src, &dst, false);

  }
}

struct UniformData
{
  float deltaTime;
  Uint32 entityCount;
};

void ObjectTransform::bind(SDL_GPUCommandBuffer* cmd)
{
  entityCount = exist->size();
  if (!computePipeline.pipeline() || entityCount == 0) return;

  Uint32 groupCount = (entityCount + 64 - 1) / 64;
  UniformData uniformData = {*secondPerFrame, entityCount};

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

void ObjectTransform::bindG(SDL_GPURenderPass* pass, uint32_t slot)
{
  if (exist->size() <= 0) return;
  SDL_GPUBuffer* bufs[] = { modelBuffer.buffer() };

  SDL_BindGPUVertexStorageBuffers( pass, slot, bufs, 1);
}