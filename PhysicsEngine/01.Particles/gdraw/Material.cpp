//
// Created by Admin on 15/01/2025.
//

#include "Material.hpp"
#include <SDL3/SDL.h>
#include "GpuUploader.hpp"
#include "VertexTypes.hpp"

namespace gdraw
{
    Material::Material(SDL_GPUDevice* device_) : device{device_} {}
    Material::~Material() { Clear(); }

    void Material::LoadShader(const str& shaderFilename, u32 samplerCount,
                              u32 uniformBufferCount, u32 storageBufferCount, u32 storageTextureCount) {
        const char* basePath = SDL_GetBasePath();

        // Auto-detect the shader stage from the file name for convenience
        SDL_GPUShaderStage stage;
        if (SDL_strstr(shaderFilename.c_str(), ".vert")) { stage = SDL_GPU_SHADERSTAGE_VERTEX; } else if (
            SDL_strstr(shaderFilename.c_str(), ".frag")) { stage = SDL_GPU_SHADERSTAGE_FRAGMENT; } else
        {
            SDL_Log("Invalid shader stage!");
        }

        char fullPath[256];
        SDL_GPUShaderFormat backendFormats = SDL_GetGPUShaderFormats(device);
        SDL_GPUShaderFormat format = SDL_GPU_SHADERFORMAT_INVALID;
        const char* entrypoint;

        if (backendFormats & SDL_GPU_SHADERFORMAT_SPIRV)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/SPIRV/%s.spv", basePath,
                         shaderFilename);
            format = SDL_GPU_SHADERFORMAT_SPIRV;
            entrypoint = "main";
        } else if (backendFormats & SDL_GPU_SHADERFORMAT_MSL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/MSL/%s.msl", basePath, shaderFilename);
            format = SDL_GPU_SHADERFORMAT_MSL;
            entrypoint = "main0";
        } else if (backendFormats & SDL_GPU_SHADERFORMAT_DXIL)
        {
            SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Shaders/Compiled/DXIL/%s.dxil", basePath,
                         shaderFilename);
            format = SDL_GPU_SHADERFORMAT_DXIL;
            entrypoint = "main";
        } else { SDL_Log("%s", "Unrecognized backend shader format!"); }

        size_t codeSize;
        void* code = SDL_LoadFile(fullPath, &codeSize);
        if (code == nullptr) { SDL_Log("Failed to load shader from disk! %s", fullPath); }

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
        if (shader == nullptr)
        {
            SDL_Log("Failed to create shader!");
            SDL_free(code);
        }

        SDL_free(code);
        shader = loadedShader;
    }

    void Material::LoadTexture(const char* imageFilename, int desiredChannels) {
        // Load surface
        const char* basePath = SDL_GetBasePath();
        char fullPath[256];
        SDL_PixelFormat format;

        SDL_snprintf(fullPath, sizeof(fullPath), "%sContent/Images/%s", basePath, imageFilename);
        SDL_Surface* surface = SDL_LoadBMP(fullPath);
        if (surface == nullptr) { SDL_Log("Failed to load BMP: %s", SDL_GetError()); }
        if (desiredChannels == 4) { format = SDL_PIXELFORMAT_ABGR8888; } else
        {
            SDL_assert(!"Unexpected desiredChannels");
            SDL_DestroySurface(surface);
        }
        if (surface->format != format)
        {
            SDL_Surface* next = SDL_ConvertSurface(surface, format);
            SDL_DestroySurface(surface);
            surface = next;
        }
        if (surface == nullptr) { SDL_Log("Could not load image data!"); }

        // Create texture
        SDL_GPUTextureCreateInfo textureInfo
        {
            .type = SDL_GPU_TEXTURETYPE_2D,
            .format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
            .usage = SDL_GPU_TEXTUREUSAGE_SAMPLER,
            .width = static_cast<Uint32>(surface->w),
            .height = static_cast<Uint32>(surface->h),
            .layer_count_or_depth = 1,
            .num_levels = 1,
        };
        texture = SDL_CreateGPUTexture(device, &textureInfo);

        // Upload to GPU
        GPUUploader uploader{device};
        uploader.PrepareTextureData<PositionTextureVertex>(surface);
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
    }
} // gdraw
