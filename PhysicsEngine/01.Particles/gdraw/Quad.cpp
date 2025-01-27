//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#include "Quad.hpp"

#include <VertexTypes.hpp>
//#include <Mat4.hpp>

namespace gdraw
{
    void Quad::Load() {
        material.LoadVertexShader("TexturedQuad.vert", 0, 0, 0, 0);
        material.LoadFragmentShader("TexturedQuad.frag", 1, 0, 0, 0);
        material.LoadTexture("ravioli.bmp", 4);
        material.CreateSampler(TextureFilter::Nearest, TextureWrap::ClampToEdge);
        material.CreatePipeline();
        mesh.Load();
    }

    void Quad::Update(f32 dt) {
        time += dt;
        Mat4 matrixUniform = Mat4::CreateRotationZ(time) * Mat4::CreateTranslation(0.0f, 0.0f, 0);
        mesh.SetTransform(matrixUniform);
    }

    void Quad::Bind() {
        material.Bind();
        mesh.Bind();
    }

    void Quad::Draw() {
        Bind();
        mesh.Draw();
    }

    void Quad::Unload() {
        mesh.Unload();
        material.Unload();
    }
}
