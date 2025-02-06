//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#ifndef GASSETS_GPUUPLOADER_HPP
#define GASSETS_GPUUPLOADER_HPP


#include <cstring>
#include <Defines.hpp>
#include <SDL3/SDL_gpu.h>

namespace gassets
{
    class GPUUploader {
    public:
        explicit GPUUploader(SDL_GPUDevice* device_);

        void PrepareTextureData(SDL_Surface* surface);
        void PrepareTransferBuffer(u32 size);
        void* MapTransferBuffer(bool cycle) const;
        void UnmapTransferBuffer() const ;

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
}

#endif //GASSETS_GPUUPLOADER_HPP
