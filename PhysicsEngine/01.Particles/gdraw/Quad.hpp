//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#ifndef GDRAW_QUAD_HPP
#define GDRAW_QUAD_HPP

#include "Material.hpp"
#include "QuadMesh.hpp"
#include "Mat4.h"

using gphysics::Mat4;

namespace gdraw
{
    class Quad
    {
    public:
        explicit Quad(Renderer& renderer_) : material(&renderer_), mesh(&renderer_) {}

        void Load();
        void Update(f32 dt);
        void Draw();
        void Unload();

    private:
        Material material;
        QuadMesh mesh;
        Mat4 transform;
        float time;;

        void Bind();
    };
}


#endif //GDRAW_QUAD_HPP
