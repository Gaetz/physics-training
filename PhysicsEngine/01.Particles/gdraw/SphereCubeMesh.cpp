//
// Created by Gaëtan Blaise-Cazalet on 28/01/2025.
//

#include "SphereCubeMesh.hpp"

#include <GpuUploader.hpp>
#include <Renderer.hpp>
#include <VertexTypes.hpp>
#include <Mat4.hpp>
#include <Vec3.hpp>

namespace gdraw
{
    SphereCubeMesh::SphereCubeMesh(Renderer* renderer_) : renderer{renderer_} {
        // Create the vertex buffer
        SDL_GPUBufferCreateInfo vertexBufferCreateInfo =
        {
            .usage = SDL_GPU_BUFFERUSAGE_VERTEX,
            .size = sizeof(PositionTextureVertex) * 8
        };
        vertexBuffer = renderer->CreateBuffer(vertexBufferCreateInfo);

        // Create the index buffer
        SDL_GPUBufferCreateInfo indexBufferCreateInfo =
        {
            .usage = SDL_GPU_BUFFERUSAGE_INDEX,
            .size = sizeof(Uint16) * 36
        };
        indexBuffer = renderer->CreateBuffer(indexBufferCreateInfo);
    }

    void SphereCubeMesh::Load() const {
        GPUUploader uploader{renderer->device};
        uploader.PrepareTransferBuffer((sizeof(PositionTextureVertex) * 8) + (sizeof(u16) * 36));

        // Map the transfer buffer and fill it with data (data is bound to the transfer buffer)
        auto transferData = static_cast<PositionTextureVertex*>(uploader.MapTransferBuffer(false));

        transferData[0] = PositionTextureVertex{ -0.5f, -0.5f,  0.5f, 0, 0 };
        transferData[1] = PositionTextureVertex{ 0.5f, -0.5f,  0.5f, 1, 0 };
        transferData[2] = PositionTextureVertex{ 0.5f,  0.5f,  0.5f, 1, 1 };
        transferData[3] = PositionTextureVertex{ -0.5f,  0.5f,  0.5f, 0, 1 };

        transferData[4] = PositionTextureVertex{ -0.5f, -0.5f, -0.5f, 0, 0 };
        transferData[5] = PositionTextureVertex{ 0.5f, -0.5f, -0.5f, 1, 0 };
        transferData[6] = PositionTextureVertex{ 0.5f,  0.5f, -0.5f, 1, 1 };
        transferData[7] = PositionTextureVertex{ -0.5f,  0.5f, -0.5f, 0, 1 };

        auto indexData = reinterpret_cast<u16*>(&transferData[8]);
        u16 indices[] = {
                // Front
                0, 1, 2,
                0, 2, 3,

                // Back
                4, 6, 5,
                4, 7, 6,

                // Left
                4, 0, 3,
                4, 3, 7,

                // Right
                1, 5, 6,
                1, 6, 2,

                // Bottom
                4, 5, 1,
                4, 1, 0,

                // Top
                3, 2, 6,
                3, 6, 7
        };
        std::memcpy(indexData, indices, sizeof(indices));

        uploader.UnmapTransferBuffer();

        // Upload the transfer data to the vertex and index buffer
        SDL_GPUTransferBufferLocation transferVertexBufferLocation
        {
            .transfer_buffer = uploader.GetTransferBuffer(),
            .offset = 0
        };
        SDL_GPUBufferRegion vertexBufferRegion
        {
            .buffer = vertexBuffer,
            .offset = 0,
            .size = sizeof(PositionTextureVertex) * 8
        };
        SDL_GPUTransferBufferLocation transferIndexBufferLocation
        {
            .transfer_buffer = uploader.GetTransferBuffer(),
            .offset = sizeof(PositionTextureVertex) * 8
        };
        SDL_GPUBufferRegion indexBufferRegion
        {
            .buffer = indexBuffer,
            .offset = 0,
            .size = sizeof(u16) * 36
        };
        uploader.Begin();
        uploader.UploadToBuffer(transferVertexBufferLocation, vertexBufferRegion, false);
        uploader.UploadToBuffer(transferIndexBufferLocation, indexBufferRegion, false);
        uploader.End();
    }

    void SphereCubeMesh::Bind() {
        SDL_GPUBufferBinding vertexBindings{.buffer = vertexBuffer, .offset = 0};
        renderer->BindVertexBuffers(0, vertexBindings, 1);

        SDL_GPUBufferBinding indexBindings{.buffer = indexBuffer, .offset = 0};
        renderer->BindIndexBuffer(indexBindings, SDL_GPU_INDEXELEMENTSIZE_16BIT);
    }

    void SphereCubeMesh::Draw() {
        renderer->DrawIndexedPrimitives(36, 1, 0, 0, 0);
    }

    void SphereCubeMesh::Unload() {
        Clear();
    }

    void SphereCubeMesh::SetTransform(const Mat4& transform) {
        renderer->PushVertexUniformData(0, &transform, sizeof(transform));
    }

    void SphereCubeMesh::Clear() {
        renderer->ReleaseBuffer(vertexBuffer);
        renderer->ReleaseBuffer(indexBuffer);
    }

    i32 SphereCubeMesh::SetQuadIndices(vector<i32> &indices, i32 i, i32 v00, i32 v10, i32 v01, i32 v11) {
        indices[i] = v00;
        indices[i + 1] = indices[i + 4] = v01;
        indices[i + 2] = indices[i + 3] = v10;
        indices[i + 5] = v11;
        return i + 6;
    }

    i32 SphereCubeMesh::CreateTopFace(vector<i32> &indices, i32 t, i32 ring) {
        i32 v = ring * gridSize;
        for (i32 x = 0; x < gridSize - 1; x++, v++) {
            t = SetQuadIndices(indices, t, v, v + 1, v + ring - 1, v + ring);
        }
        t = SetQuadIndices(indices, t, v, v + 1, v + ring - 1, v + 2);

        i32 vMin = ring * (gridSize + 1) - 1;
        i32 vMid = vMin + 1;
        i32 vMax = v + 2;

        for (i32 z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(indices, t, vMin, vMid, vMin - 1, vMid + gridSize - 1);
            for (i32 x = 1; x < gridSize - 1; x++, vMid++) {
                t = SetQuadIndices(indices, t, vMid, vMid + 1, vMid + gridSize - 1, vMid + gridSize);
            }
            t = SetQuadIndices(indices, t, vMid, vMax, vMid + gridSize - 1, vMax + 1);
        }

        i32 vTop = vMin - 2;
        t = SetQuadIndices(indices, t, vMin, vMid, vTop + 1, vTop);
        for (i32 x = 1; x < gridSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(indices, t, vMid, vMid + 1, vTop, vTop - 1);
        }
        t = SetQuadIndices(indices, t, vMid, vTop - 2, vTop, vTop - 1);

        return t;
    }

    i32 SphereCubeMesh::CreateBottomFace(vector<i32> &indices, i32 vertexCount, i32 t, i32 ring) {
        i32 v = 1;
        i32 vMid = vertexCount - (gridSize - 1) * (gridSize - 1);
        t = SetQuadIndices(indices, t, ring - 1, vMid, 0, 1);
        for (i32 x = 1; x < gridSize - 1; x++, v++, vMid++) {
            t = SetQuadIndices(indices, t, vMid, vMid + 1, v, v + 1);
        }
        t = SetQuadIndices(indices, t, vMid, v + 2, v, v + 1);

        i32 vMin = ring - 2;
        vMid -= gridSize - 2;
        i32 vMax = v + 2;

        for (i32 z = 1; z < gridSize - 1; z++, vMin--, vMid++, vMax++) {
            t = SetQuadIndices(indices, t, vMin, vMid + gridSize - 1, vMin + 1, vMid);
            for (i32 x = 1; x < gridSize - 1; x++, vMid++) {
                t = SetQuadIndices(indices, t, vMid + gridSize - 1, vMid + gridSize, vMid, vMid + 1);
            }
            t = SetQuadIndices(indices, t, vMid + gridSize - 1, vMax + 1, vMid, vMax);
        }

        i32 vTop = vMin - 1;
        t = SetQuadIndices(indices, t, vTop + 1, vTop, vTop + 2, vMid);
        for (i32 x = 1; x < gridSize - 1; x++, vTop--, vMid++) {
            t = SetQuadIndices(indices, t, vTop, vTop - 1, vMid, vMid + 1);
        }
        t = SetQuadIndices(indices, t, vTop, vTop - 1, vMid, vTop - 2);

        return t;
    }

    void SphereCubeMesh::SetVertex(vector<Vec3> &vertices, vector<Vec3> &normals, i32 i, i32 x, i32 y, i32 z) {
        Vec3 v = Vec3 { static_cast<f32>(x), static_cast<f32>(y), static_cast<f32>(z) } * 2.0f / gridSize - Vec3::one;
        normals[i] = v.Normalize();
        vertices[i] = normals[i] * radius;
    }

    void SphereCubeMesh::Build() {
        // Setup geometry data
        i32 cornerVertices { 8 };
        i32 edgeVertices { (gridSize + gridSize + gridSize - 3) * 4 };
        i32 faceVertices { ((gridSize - 1) * (gridSize - 1) + (gridSize - 1) * (gridSize - 1) + (gridSize - 1) * (gridSize - 1)) * 2 };
        const i32 vertexCount { cornerVertices + edgeVertices + faceVertices };
        i32 quadsCount = (gridSize * gridSize + gridSize * gridSize + gridSize * gridSize) * 2;

        vertices.reserve(vertexCount);
        vertices.resize(vertexCount);
        normals.reserve(vertexCount);
        normals.resize(vertexCount);
        indices.reserve(quadsCount * 6);
        indices.resize(quadsCount * 6);

        // Generate vertices
        i32 v { 0 };
        // -- Lateral faces
        for (i32 y = 0; y <= gridSize; y++) {
            for (i32 x = 0; x <= gridSize; x++) {
                SetVertex(vertices, normals, v++, x, y, 0);
            }
            for (i32 z = 1; z <= gridSize; z++) {
                SetVertex(vertices, normals, v++, gridSize, y, z);
            }
            for (i32 x = gridSize - 1; x >= 0; x--) {
                SetVertex(vertices, normals, v++, x, y, gridSize);
            }
            for (i32 z = gridSize - 1; z > 0; z--) {
                SetVertex(vertices, normals, v++, 0, y, z);
            }
        }
        // -- Top and bottom
        for (i32 z = 1; z < gridSize; z++) {
            for (i32 x = 1; x < gridSize; x++) {
                SetVertex(vertices, normals, v++, x, gridSize, z);
            }
        }
        for (i32 z = 1; z < gridSize; z++) {
            for (i32 x = 1; x < gridSize; x++) {
                SetVertex(vertices, normals, v++, x, 0, z);
            }
        }

        // Generate indices
        i32 ring { (gridSize + gridSize) * 2 };
        i32 t { 0 };
        v = 0;
        // -- Lateral faces
        for (i32 y = 0; y < gridSize; y++, v++) {
            for (i32 q = 0; q < ring - 1; q++, v++) {
                t = SetQuadIndices(indices, t, v, v + 1, v + ring, v + ring + 1);
            }
            t = SetQuadIndices(indices, t, v, v - ring + 1, v + ring, v + 1);
        }
        // -- Top and bottom
        t = CreateTopFace(indices, t, ring);
        t = CreateBottomFace(indices, vertexCount, t, ring);
    }
}
