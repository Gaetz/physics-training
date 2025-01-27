//
// Created by Gaëtan Blaise-Cazalet on 15/01/2025.
//

#include "Quad.hpp"

#include <VertexTypes.hpp>
//#include <Mat4.hpp>

namespace gdraw
{
    void Quad::Load() {
        mesh.Load();
        material.LoadVertexShader("TexturedQuadWithMatrix.vert", 0, 1, 0, 0);
        material.LoadFragmentShader("TexturedQuadWithMultiplyColor.frag", 1, 1, 0, 0);
        material.CreateSampler(TextureFilter::Linear, TextureWrap::ClampToEdge);
        material.LoadTexture("ravioli.bmp", 4);
        material.CreatePipeline();
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
