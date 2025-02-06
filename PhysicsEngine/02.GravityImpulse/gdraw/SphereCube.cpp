//
// Created by Gaëtan Blaise-Cazalet on 31/01/2025.
//

#include "SphereCube.hpp"

#include <VertexTypes.hpp>
#include <RendererSdl.hpp>

namespace gdraw
{
    void SphereCube::Load() {
        material.LoadVertexShader("PositionNormalTransform.vert", 0, 1, 0, 0);
        material.LoadFragmentShader("ColoredShape.frag", 1, 1, 0, 0);
        material.LoadTexture("cube.bmp", 4);
        material.CreateSampler(TextureFilter::Nearest, TextureWrap::ClampToEdge);
        material.CreatePipeline();
        mesh.Load();
    }

    void SphereCube::Update(f32 dt) {

    }

    void SphereCube::Bind() {
        material.Bind();
        mesh.Bind();
    }

    void SphereCube::Draw(const Renderer& renderer) {
        mesh.SetTransform(transform, renderer.GetView(), renderer.GetProjection());
        Bind();
        mesh.Draw();
    }

    void SphereCube::Unload() {
        mesh.Unload();
        material.Unload();
    }

    void SphereCube::SetTransform(const Vec &position, const Quat &rotation, const Vec &scale) {
        transform = Mat4::CreateTranslation(position) * Mat4::CreateFromQuaternion(rotation) * Mat4::CreateScale(scale);
    }

    void SphereCube::SetTransform(const Mat4 &transform_) {
        transform = transform_;
    }
}
