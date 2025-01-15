//
// Created by Admin on 15/01/2025.
//

#ifndef GDRAW_GPUUPLOADER_HPP
#define GDRAW_GPUUPLOADER_HPP

#include <SDL3/SDL_gpu.h>
#include <cstring>

class GPUUploader {
public:
    explicit GPUUploader(SDL_GPUDevice* device_);
    ~GPUUploader();

    template<typename T>
    void PrepareTextureData(SDL_Surface* surface) {
        Uint32 bufferSize = surface->w * surface->h * 4;
        SDL_GPUTransferBufferCreateInfo textureTransferBufferCreateInfo{
            .usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
            .size = bufferSize
        };
        SDL_GPUTransferBuffer* textureTransferBuffer = SDL_CreateGPUTransferBuffer(
            device, &textureTransferBufferCreateInfo);
        auto textureTransferData = static_cast<T*>(SDL_MapGPUTransferBuffer(
            device, textureTransferBuffer, false));
        std::memcpy(textureTransferData, surface->pixels, surface->w * surface->h * 4);
        SDL_UnmapGPUTransferBuffer(device, textureTransferBuffer);
    }

    void Begin();
    void UploadToBuffer(const SDL_GPUTransferBufferLocation& source, const SDL_GPUBufferRegion& destination, bool cycle) const;
    void UploadToTexture(const SDL_GPUTextureTransferInfo& source, const SDL_GPUTextureRegion& destination, bool cycle) const;
    void End();

    SDL_GPUTransferBuffer* GetTransferBuffer() const { return transferBuffer; }

private:
    SDL_GPUDevice* device {nullptr};
    SDL_GPUTransferBuffer* transferBuffer {nullptr};
    SDL_GPUCommandBuffer* uploadCmdBuf {nullptr};
    SDL_GPUCopyPass* copyPass {nullptr};
};



#endif //GDRAW_GPUUPLOADER_HPP
