#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <cstdio>
#include <loader/BW.h>

#include "objectTransform.h"

void ObjectTransform::saveData(SDL_GPUCommandBuffer* cmd)
{
  TransferBuffer savePositionTransfer;
  TransferBuffer saveScalingTransfer ;
  TransferBuffer saveRotationTransfer;

  SDL_GPUCopyPass* copypass = SDL_BeginGPUCopyPass(cmd);

  savePositionTransfer.downloadData(device, copypass, positionBuffer.buffer(), visible->size() * sizeof(glm::vec4), 0, 0);
  saveScalingTransfer .downloadData(device, copypass, scalingBuffer .buffer(), visible->size() * sizeof(glm::vec4), 0, 0);
  saveRotationTransfer.downloadData(device, copypass, rotationBuffer.buffer(), visible->size() * sizeof(glm::vec4), 0, 0);

  SDL_EndGPUCopyPass(copypass);

  SDL_GPUFence* fence = SDL_SubmitGPUCommandBufferAndAcquireFence(cmd);
  SDL_WaitForGPUFences(device, true, &fence, 1);
  SDL_ReleaseGPUFence(device, fence);



  glm::vec4* translateData = (glm::vec4*)SDL_MapGPUTransferBuffer(device, savePositionTransfer.buffer(), false);
  glm::vec4* scalingData   = (glm::vec4*)SDL_MapGPUTransferBuffer(device, saveScalingTransfer .buffer(), false);
  glm::vec4* rotationData  = (glm::vec4*)SDL_MapGPUTransferBuffer(device, saveRotationTransfer.buffer(), false);

  rapidjson::StringBuffer buffer;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> w(buffer);
  
  w.SetIndent(' ', 2);
  w.StartObject();
  w.Key("count");
  w.Int(visible->size());
  w.Key("position");
  w.StartObject();
  w.Key("x");
  w.StartArray();

  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);
  for (int a = 0; a < visible->size(); a ++)
    w.Double(translateData[a].x);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(translateData[a].y);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(translateData[a].z);
  w.EndArray();
  w.EndObject();

  w.Key("scale");
  w.StartObject();

  w.Key("x");
  w.StartArray();
  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);

  for (int a = 0; a < visible->size(); a ++)
    w.Double(scalingData[a].x);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(scalingData[a].y);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(scalingData[a].z);
  w.EndArray();
  w.EndObject();

  w.Key("rotation");
  w.StartObject();

  w.Key("x");
  w.StartArray();
  w.SetFormatOptions(rapidjson::kFormatSingleLineArray);
  w.SetMaxDecimalPlaces(1);
  for (int a = 0; a < visible->size(); a ++)
    w.Double(rotationData[a].x);
  w.EndArray();

  w.Key("y");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(rotationData[a].y);
  w.EndArray();

  w.Key("z");
  w.StartArray();
  for (int a = 0; a < visible->size(); a ++)
    w.Double(rotationData[a].z);
  w.EndArray();
  w.EndObject();

  w.EndObject();
  BW::File fp = {"../src/data/json/games/entitydata.json", "wb"};
  if (fp.fp)
  {
    fwrite(buffer.GetString(), 1, buffer.GetSize(), fp.fp);
  }

  SDL_UnmapGPUTransferBuffer(device, savePositionTransfer.buffer());
  SDL_UnmapGPUTransferBuffer(device, saveScalingTransfer .buffer());
  SDL_UnmapGPUTransferBuffer(device, saveRotationTransfer.buffer());
}