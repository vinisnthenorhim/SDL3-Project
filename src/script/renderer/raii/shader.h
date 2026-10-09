#pragma once

#include <SDL3/SDL.h>
#include "check.h"

class Shader 
{
  public:
    Shader(SDL_GPUDevice* device, const char* base, SDL_GPUShaderStage stage, Uint32 uniBuffer = 0,  Uint32 storageBuff = 0)
    {
      this->device = device;
      shad = loadShader( device, base, stage, uniBuffer, storageBuff);
    }
    ~Shader()
    {
      release();
    }
    SDL_GPUShader* shader() { return shad; }
    bool valid() const { return shad != nullptr; }
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;
    Shader(Shader&& o) noexcept : device(o.device), shad(o.shad) { o.shad = nullptr; }
    Shader& operator=(Shader&& o) noexcept {
        if (this != &o) {
            release();
            device = o.device; 
            shad = o.shad;
            o.shad = nullptr;
        }
        return *this;
    }
  private:
    SDL_GPUDevice* device = nullptr;
    SDL_GPUShader* shad   = nullptr;
    SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* base, SDL_GPUShaderStage stage, Uint32 uniBuffer, Uint32 storageBuff)
    {
        // 1. ask the backend which format it accepts
        SDL_GPUShaderFormat supported = SDL_GetGPUShaderFormats(device);

        SDL_GPUShaderFormat format;
        const char* ext;
        const char* entry = "main";

        if (supported & SDL_GPU_SHADERFORMAT_SPIRV)     { format = SDL_GPU_SHADERFORMAT_SPIRV; ext = ".spv"; }
        else if (supported & SDL_GPU_SHADERFORMAT_DXIL) { format = SDL_GPU_SHADERFORMAT_DXIL;  ext = ".dxil"; }
        else if (supported & SDL_GPU_SHADERFORMAT_MSL)  { format = SDL_GPU_SHADERFORMAT_MSL;   ext = ".msl"; entry = "main0"; }
        else { SDL_Log("No supported shader format"); return nullptr; }

        // 2. build the file name: "tri.vert" -> "tri.vert.spv"
        char path[256];
        SDL_snprintf(path, sizeof(path), "%s%s", base, ext);

        // 3. read the file
        size_t size = 0;
        void* code = SDL_LoadFile(path, &size);
        if (!code) {
            SDL_Log("Load %s failed: %s", path, SDL_GetError());
            return nullptr;
        }

        // 4. create the shader
        SDL_GPUShaderCreateInfo info = {};
        info.code       = (const Uint8*)code;
        info.code_size  = size;
        info.entrypoint = entry;
        info.format     = format;
        info.stage      = stage;
        info.num_uniform_buffers = uniBuffer;
        info.num_storage_buffers = storageBuff;

        SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
        SDL_free(code);

        CHECK(shader);
        return shader;
    }
    void release() {
        if (shad) SDL_ReleaseGPUShader(device, shad);
        shad = nullptr;
    }
};


class ComputeShader 
{
  public:
    ComputeShader(SDL_GPUDevice* device, const char* base, Uint32 uniBuffer = 0,  Uint32 storageBuff = 0)
    {
      this->device = device;
      shad = loadShader( device, base, uniBuffer, storageBuff);
    }
    ~ComputeShader()
    {
      release();
    }
    SDL_GPUShader* shader() { return shad; }
    bool valid() const { return shad != nullptr; }
    ComputeShader(const ComputeShader&) = delete;
    ComputeShader& operator=(const ComputeShader&) = delete;
    ComputeShader(ComputeShader&& o) noexcept : device(o.device), shad(o.shad) { o.shad = nullptr; }
    ComputeShader& operator=(ComputeShader&& o) noexcept {
        if (this != &o) {
            release();
            device = o.device; 
            shad = o.shad;
            o.shad = nullptr;
        }
        return *this;
    }
  private:
    SDL_GPUDevice* device = nullptr;
    SDL_GPUShader* shad   = nullptr;
    SDL_GPUShader* loadShader(SDL_GPUDevice* device, const char* base, Uint32 uniBuffer, Uint32 storageBuff)
    {
        // 1. ask the backend which format it accepts
        SDL_GPUShaderFormat supported = SDL_GetGPUShaderFormats(device);

        SDL_GPUShaderFormat format;
        const char* ext;
        const char* entry = "main";

        if (supported & SDL_GPU_SHADERFORMAT_SPIRV)     { format = SDL_GPU_SHADERFORMAT_SPIRV; ext = ".spv"; }
        else if (supported & SDL_GPU_SHADERFORMAT_DXIL) { format = SDL_GPU_SHADERFORMAT_DXIL;  ext = ".dxil"; }
        else if (supported & SDL_GPU_SHADERFORMAT_MSL)  { format = SDL_GPU_SHADERFORMAT_MSL;   ext = ".msl"; entry = "main0"; }
        else { SDL_Log("No supported shader format"); return nullptr; }

        // 2. build the file name: "tri.vert" -> "tri.vert.spv"
        char path[256];
        SDL_snprintf(path, sizeof(path), "%s%s", base, ext);

        // 3. read the file
        size_t size = 0;
        void* code = SDL_LoadFile(path, &size);
        if (!code) {
            SDL_Log("Load %s failed: %s", path, SDL_GetError());
            return nullptr;
        }

        // 4. create the shader
        SDL_GPUShaderCreateInfo info = {};
        info.code       = (const Uint8*)code;
        info.code_size  = size;
        info.entrypoint = entry;
        info.format     = format;
        info.num_uniform_buffers = uniBuffer;
        info.num_storage_buffers = storageBuff;

        SDL_GPUShader* shader = SDL_CreateGPUShader(device, &info);
        SDL_free(code);

        CHECK(shader);
        return shader;
    }
    void release() {
        if (shad) SDL_ReleaseGPUShader(device, shad);
        shad = nullptr;
    }
};