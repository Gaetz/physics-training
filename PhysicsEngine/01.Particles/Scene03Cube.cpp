#include "Scene03Cube.hpp"

void Scene03Cube::Load(Renderer& renderer) {
    quad = std::make_unique<Quad>(renderer);
    quad->Load();
}

bool Scene03Cube::Update(float dt) {
    bool stillRunning = ManageInput(inputState);
    quad->Update(dt);
    return stillRunning;
}

void Scene03Cube::Draw(Renderer& renderer) {
    renderer.Begin();

    quad->Draw();

    renderer.End();
}

void Scene03Cube::Unload(Renderer& renderer) {
    quad->Unload();
}