//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#include "Quad.hpp"

#include <VertexTypes.hpp>

namespace gdraw
{
    void Quad::Load() {
        material.LoadVertexShader("TexturedQuadWithMatrix.vert", 0, 1, 0, 0);
        material.LoadFragmentShader("TexturedQuadWithMultiplyColor.frag", 1, 1, 0, 0);
        material.LoadTexture("ravioli.bmp", 4);
        material.CreateSampler(TextureFilter::Nearest, TextureWrap::ClampToEdge);
        material.CreatePipeline();
        mesh.Load();
    }

    void Quad::Update(f32 dt) {
        time += dt;
        transform = Mat4::CreateRotationZ(time) * Mat4::CreateTranslation(0.0f, 0.0f, 0);
    }

    void Quad::Bind() {
        material.Bind();
        mesh.Bind();
        mesh.SetTransform(transform);
    }

    void Quad::Draw(const Renderer& renderer) {
        Bind();
        mesh.Draw();
    }

    void Quad::Unload() {
        mesh.Unload();
        material.Unload();
    }

    void Quad::SetTransform(const Vec &position, const Quat &rotation, const Vec &scale) {
        transform = Mat4::CreateTranslation(position) * Mat4::CreateFromQuaternion(rotation) * Mat4::CreateScale(scale);
    }

    void Quad::SetTransform(const Mat4 &transform_) {
        transform = transform_;
    }
}
