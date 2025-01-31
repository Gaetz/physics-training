//
// Created by Gaëtan Blaise-Cazalet on 28/01/2025.
//

#ifndef GDRAW_SPHERECUBEMESH_HPP
#define GDRAW_SPHERECUBEMESH_HPP
#include <SDL3/SDL_gpu.h>
#include "Defines.hpp"

namespace gmath {
    class Mat4;
    class Vec3;
}

using gmath::Mat4;
using gmath::Vec3;

namespace gdraw
{
    class Renderer;

    class SphereCubeMesh
    {
    public:
        explicit SphereCubeMesh(Renderer* renderer_);
        void Load();
        void Bind();
        void Draw();
        void Unload();
        void SetTransform(const Mat4& transform_);

    private:
        Renderer* renderer{nullptr};
        vector<Vec3> vertices;
        vector<Vec3> normals;
        vector<i32> indices;
        u32 vertexCount;
        u32 quadCount;
        SDL_GPUBuffer* vertexBuffer{ nullptr };
        SDL_GPUBuffer* indexBuffer{ nullptr };

        i32 gridSize { 1 };
        f32 radius { 1.0f };

        void Build();
        void Rebuild();

        i32 SetQuadIndices(vector<i32>& indices, i32 i, i32 v00, i32 v10, i32 v01, i32 v11);

        i32 CreateTopFace(vector<i32>& indices, i32 t, i32 ring);

        i32 CreateBottomFace(vector<i32>& indices, i32 vertexCount, i32 t, i32 ring);

        void SetVertex(vector<Vec3>& vertices, vector<Vec3>& normals, i32 i, i32 x, i32 y, i32 z);

        void Clear();
    };
}


#endif //GDRAW_SPHERECUBEMESH_HPP
