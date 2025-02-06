//
// Created by Admin on 06/02/2025.
//

#include "AssetsManager.hpp"

#include <Folder.hpp>
#include <Log.hpp>
#include <File.hpp>
#include <FileTypes.hpp>
#include <Asserts.hpp>
#include "SDL3/SDL_filesystem.h"
#include "GpuUploader.hpp"

using gfile::Folder;
using gfile::File;
using gfile::FileType;

namespace gassets
{
    void AssetsManager::Initialize(const str& gameAssetsPath, SDL_GPUDevice* device_) {
        device = device_;
        // Setting default folders
        if (!Folder::Exists(gameAssetsPath))
        {
            LOG(LogLevel::Warning) << "No data in game data path";
            return;
        }
        File::SetGameAssetsPath(gameAssetsPath);
        // Default assets
        LoadTexture(DEFAULT_TEXTURE_ID, "invalid-texture.png", TextureFilter::Nearest, TextureWrap::Repeat);
    }

    constexpr i32 desiredChannels = 4;

    void AssetsManager::LoadTexture(const str& name, const str& filename, TextureFilter filter, TextureWrap wrap) {
        // Load surface
        SDL_PixelFormat format;
        str fullPath = name == DEFAULT_TEXTURE_ID
                           ? (gfile::FileTypePath(gfile::FileType::System) + filename)
                           : (gfile::FileTypePath(gfile::FileType::Texture) + filename);
        SDL_Surface* surface = SDL_LoadBMP(fullPath.c_str());
        if (surface == nullptr) { LOG(LogLevel::Warning) << "Failed to load BMP: " << SDL_GetError(); }

        GASSERT_MSG(desiredChannels == 4, "Unexpected desiredChannels. Should be 4.");
        if constexpr (desiredChannels == 4) { format = SDL_PIXELFORMAT_ABGR8888; } else { SDL_DestroySurface(surface); }
        if (surface->format != format)
        {
            SDL_Surface* next = SDL_ConvertSurface(surface, format);
            SDL_DestroySurface(surface);
            surface = next;
        }
        if (surface == nullptr) { LOG(LogLevel::Warning) << "Could not load image data!"; }

        // Create texture
        const SDL_GPUTextureCreateInfo textureInfo
        {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
            .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
            .width = static_cast<Uint32>(surface->w),
            .height = static_cast<Uint32>(surface->h),
            .layer_count_or_depth = 1,
            .num_levels = 1,
        };
        SDL_GPUTexture* texture = SDL_CreateGPUTexture(device, &textureInfo);

        // Upload to GPU
        GPUUploader uploader{device};
        uploader.PrepareTextureData(surface);
        uploader.Begin();
        SDL_GPUTextureTransferInfo textureBufferLocation{
            .transfer_buffer = uploader.GetTransferBuffer(),
            .offset = 0
        };
        SDL_GPUTextureRegion textureBufferRegion{
            .texture = texture,
            .w = static_cast<Uint32>(surface->w),
            .h = static_cast<Uint32>(surface->h),
            .d = 1
        };
        uploader.UploadToTexture(textureBufferLocation, textureBufferRegion, false);
        uploader.End();

        // Destroy surface data
        SDL_DestroySurface(surface);

        // Sampler creation
        SDL_GPUSamplerCreateInfo createInfo{
            .min_filter = filter == TextureFilter::Nearest ? SDL_GPU_FILTER_NEAREST : SDL_GPU_FILTER_LINEAR,
            .mag_filter = filter == TextureFilter::Nearest ? SDL_GPU_FILTER_NEAREST : SDL_GPU_FILTER_LINEAR,
            .mipmap_mode = filter == TextureFilter::Nearest
                               ? SDL_GPU_SAMPLERMIPMAPMODE_NEAREST
                               : SDL_GPU_SAMPLERMIPMAPMODE_LINEAR
        };
        if (filter == TextureFilter::Anisotropic)
        {
            createInfo.max_anisotropy = 4;
            createInfo.enable_anisotropy = true;
        }
        if (wrap == TextureWrap::ClampToEdge)
        {
            createInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            createInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            createInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
        } else if (wrap == TextureWrap::MirroredRepeat)
        {
            createInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
            createInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
            createInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_MIRRORED_REPEAT;
        } else
        {
            createInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
            createInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
            createInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_REPEAT;
        }
        SDL_GPUSampler* sampler = SDL_CreateGPUSampler(device, &createInfo);

        textures.emplace(name, Texture{texture, sampler});
    }

    Texture& AssetsManager::GetTexture(const str& name) {
        if (!textures.contains(name))
        {
            LOG(LogLevel::Warning) << "Texture [" << name <<
                "] does not exist in AssetsManager. Returning default texture.";
            return textures.at(DEFAULT_TEXTURE_ID);
        }
        return textures.at(name);
    }

    void AssetsManager::UnloadTextures() {
        for (auto& texture : textures)
        {
            SDL_ReleaseGPUTexture(device, texture.second.texture);
            SDL_ReleaseGPUSampler(device, texture.second.sampler);
        }
    }

    void AssetsManager::LoadVertexShader(const str& vertexShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                         u32 storageBufferCount, u32 storageTextureCount) {
        if (vertexShaders.contains(vertexShaderFilename))
        {
            LOG(LogLevel::Warning) << "Vertex shader [" << vertexShaderFilename << "] already loaded.";
            return;
        }
        SDL_GPUShader* shader = LoadShader(vertexShaderFilename, samplerCount, uniformBufferCount,
                                           storageBufferCount,
                                           storageTextureCount);
        vertexShaders.emplace(vertexShaderFilename, shader);
    }

    void AssetsManager::LoadFragmentShader(const str& fragmentShaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                           u32 storageBufferCount, u32 storageTextureCount) {
        if (vertexShaders.contains(fragmentShaderFilename))
        {
            LOG(LogLevel::Warning) << "Fragment shader [" << fragmentShaderFilename << "] already loaded.";
            return;
        }
        SDL_GPUShader* shader = LoadShader(fragmentShaderFilename, samplerCount, uniformBufferCount,
                                           storageBufferCount,
                                           storageTextureCount);
        fragmentShaders.emplace(fragmentShaderFilename, shader);
    }

    SDL_GPUShader* AssetsManager::LoadShader(const str& shaderFilename, u32 samplerCount, u32 uniformBufferCount,
                                             u32 storageBufferCount, u32 storageTextureCount) const {
        // Auto-detect the shader stage from the file name for convenience
        SDL_GPUShaderStage stage{SDL_GPU_SHADERSTAGE_VERTEX};
        if (SDL_strstr(shaderFilename.c_str(), ".vert")) { stage = SDL_GPU_SHADERSTAGE_VERTEX; } else if (
            SDL_strstr(shaderFilename.c_str(), ".frag")) { stage = SDL_GPU_SHADERSTAGE_FRAGMENT; } else
        {
            LOG(LogLevel::Warning) << "Shader [" << shaderFilename << "]: Invalid shader stage!";
        }

        char fullPath[256];
        SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
        SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
        const char* entrypoint{"main"};

        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.spv",
                         gfile::FileTypePath(gfile::FileType::Shader).c_str(),
                         shaderFilename.c_str());
            format = SDL_GPU_SHADERFORMAT_SPIRV;
            entrypoint = "main";
        } else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.msl",
                         gfile::FileTypePath(gfile::FileType::Shader).c_str(),
                         shaderFilename.c_str());
            format = SDL_GPU_SHADERFORMAT_MSL;
            entrypoint = "main0";
        } else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%s/%s.dxil",
                         gfile::FileTypePath(gfile::FileType::Shader).c_str(),
                         shaderFilename.c_str());
            format = SDL_GPU_SHADERFORMAT_DXIL;
            entrypoint = "main";
        } else { LOG(LogLevel::Warning) << "Unrecognized backend shader format!"; }

        size_t codeSize;
        void* code = SDL_LoadFile(fullPath, &codeSize);
        if (code == nullptr)
        {
            LOG(LogLevel::Error) << "Failed to load shader from disk!" << fullPath;
        }

        SDL_GPUShaderCreateInfo shaderInfo = {
            .code_size = codeSize,
            .code = static_cast<Uint8*>(code),
            .entrypoint = entrypoint,
            .format = format,
            .stage = stage,
            .num_samplers = samplerCount,
            .num_storage_textures = storageTextureCount,
            .num_storage_buffers = storageBufferCount,
            .num_uniform_buffers = uniformBufferCount
        };
        SDL_GPUShader* loadedShader = SDL_CreateGPUShader(device, &shaderInfo);
        if (loadedShader == nullptr)
        {
            LOG(LogLevel::Error) << "Failed to create shader [" << fullPath << "]!" << fullPath;
            SDL_free(code);
        }

        SDL_free(code);
        return loadedShader;
    }
}
