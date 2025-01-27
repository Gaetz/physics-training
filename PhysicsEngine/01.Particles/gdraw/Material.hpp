//
// Created by Admin on 15/01/2025.
//

#ifndef GDRAW_MATERIAL_HPP
#define GDRAW_MATERIAL_HPP

#include <SDL3/SDL_gpu.h>
#include "Defines.hpp"

namespace gdraw
{
    class Renderer;

    enum class TextureFilter
    {
        Nearest,
        Linear,
        Anisotropic
    };

    enum class TextureWrap
    {
        Repeat,
        ClampToEdge,
        MirroredRepeat
    };

    class Material
    {
    public:
        explicit Material(Renderer* renderer_);

        void LoadVertexShader(const str& vertexShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                              u32 storageBufferCount, u32 storageTextureCount);
        void LoadFragmentShader(const str& fragmentShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                u32 storageBufferCount, u32 storageTextureCount);
        void LoadTexture(const str& imageFilename, int desiredChannels);
        void CreateSampler(TextureFilter filter, TextureWrap wrap);
        void CreatePipeline();
        void Bind();
        void Unload();

    private:
        Renderer* renderer{nullptr};
        SDL_GPUShader* vertexShader{nullptr};
        SDL_GPUShader* fragmentShader{nullptr};
        SDL_GPUTexture* texture{nullptr};
        SDL_GPUSampler* sampler{nullptr};
        SDL_GPUGraphicsPipeline* pipeline{nullptr};

        SDL_GPUShader* LoadShader(const str& shaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                  u32 storageBufferCount,
                                  u32 storageTextureCount);
        void Clear();
    };
} // gdraw

#endif //GDRAW_MATERIAL_HPP
