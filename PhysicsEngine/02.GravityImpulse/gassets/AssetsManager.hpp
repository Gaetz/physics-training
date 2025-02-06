//
// Created by gaetz on 06/11/2022.
//

#ifndef GASSETS_ASSETSMANAGER_HPP
#define GASSETS_ASSETSMANAGER_HPP

#include <Defines.hpp>
#include <unordered_map>
#include <SDL3/SDL_gpu.h>

using std::unordered_map;

constexpr char DEFAULT_TEXTURE_ID[] = "default";

namespace gassets {

    enum class TextureFilter {
        Nearest,
        Linear,
        Anisotropic
    };

    enum class TextureWrap {
        Repeat,
        ClampToEdge,
        MirroredRepeat
    };

    struct Texture
    {
        SDL_GPUTexture* texture;
        SDL_GPUSampler* sampler;
    };

    class AssetsManager {
    public:
        void Initialize(const str& gameAssetsPath, SDL_GPUDevice* device_);

        void LoadTexture(const str& name, const str& filename, TextureFilter filter, TextureWrap wrap);
        Texture& GetTexture(const str& name);
        void UnloadTextures();

        void LoadVertexShader(const str& vertexShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                      u32 storageBufferCount, u32 storageTextureCount);
        void LoadFragmentShader(const str& fragmentShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                u32 storageBufferCount, u32 storageTextureCount);

    private:
        unordered_map<str, Texture> textures;
        unordered_map<str, SDL_GPUShader*> vertexShaders;
        unordered_map<str, SDL_GPUShader*> fragmentShaders;

        SDL_GPUDevice* device {nullptr};

        SDL_GPUShader* LoadShader(const str& shaderFilename, u32 samplerCount, u32 uniformBufferCount,
                          u32 storageBufferCount,
                          u32 storageTextureCount) const;
    };

}



#endif //GASSETS_ASSETSMANAGER_HPP
