//
// Created by Gaëtan Blaise-Cazalet on 31/01/2025.
//

#ifndef GDRAW_SPHERECUBE_HPP
#define GDRAW_SPHERECUBE_HPP

#include "SphereCubeMaterial.hpp"
#include "SphereCubeMesh.hpp"
#include "Mat4.hpp"
#include "Drawable.hpp"

using gmath::Vec;
using gmath::Mat4;
using gmath::Quat;

namespace gdraw
{
    class SphereCube : public Drawable
    {
    public:
        explicit SphereCube(Renderer& renderer_) : material(&renderer_), mesh(&renderer_) {}

        void Load();
        void Update(f32 dt);
        void Draw(const Renderer& renderer);
        void Unload();

        void SetTransform(const Mat4& transform_) override;
        void SetTransform(const Vec& position, const Quat& rotation, const Vec& scale = Vec(1, 1, 1)) override;

    private:
        SphereCubeMaterial material;
        SphereCubeMesh mesh;
        Mat4 transform { Mat4::identity };

        void Bind();
    };
}


#endif //GDRAW_CUBE_HPP
