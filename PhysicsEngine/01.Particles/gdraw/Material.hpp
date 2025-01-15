//
// Created by Admin on 15/01/2025.
//

#ifndef GDRAW_MATERIAL_HPP
#define GDRAW_MATERIAL_HPP

#include <SDL3/SDL_gpu.h>
#include "Defines.hpp"

namespace gdraw {

class Material {
public:
    explicit Material(SDL_GPUDevice* device_);
    ~Material();

    void Clear();

    SDL_GPUDevice* device {nullptr};
    SDL_GPUShader* shader {nullptr};
    SDL_GPUTexture* texture {nullptr};
    SDL_GPUSampler* sampler {nullptr};
    SDL_GPUGraphicsPipeline* pipeline {nullptr};

    void LoadShader(const str& shaderFilename, u32 samplerCount, u32 uniformBufferCount, u32 storageBufferCount, u32 storageTextureCount);
    void LoadTexture(const char* imageFilename, int desiredChannels);
    void CreateSampler(const SDL_GPUSamplerCreateInfo& createInfo);
    void CreatePipeline(const SDL_GPUGraphicsPipelineCreateInfo& createInfo);
    void Bind();
};

} // gdraw

#endif //GDRAW_MATERIAL_HPP
