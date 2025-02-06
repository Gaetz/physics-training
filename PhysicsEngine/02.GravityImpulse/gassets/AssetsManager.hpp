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

    struct TextureSampler
    {
        SDL_GPUTexture* texture;
        SDL_GPUSampler* sampler;
    };

    class AssetsManager {
    public:
        void Initialize(const str& gameAssetsPath, SDL_GPUDevice* device_);

        void LoadTexture(const str& name, const str& filename, TextureFilter filter, TextureWrap wrap);
        TextureSampler& GetTexture(const str& name);
        void UnloadTextures();

    private:
        unordered_map<str, TextureSampler> textures;

        SDL_GPUDevice* device {nullptr};
    };

}



#endif //GASSETS_ASSETSMANAGER_HPP
