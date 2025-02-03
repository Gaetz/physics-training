#include "Scene05Bodies.hpp"

void Scene05Bodies::Load(Renderer& renderer) {
    sphere = std::make_unique<SphereCube>(renderer);
    sphere->Load();
    bodies.emplace_back({Vec{0, 0, 0}, Vec3{0, 0, 0}, Vec3{0, 0, 0}, 1.0f});
}

bool Scene05Bodies::Update(float dt) {
    bool stillRunning = ManageInput(inputState);
    sphere->Update(dt);
    return stillRunning;
}

void Scene05Bodies::Draw(Renderer& renderer) {
    renderer.Begin();

    sphere->Draw(renderer);

    renderer.End();
}

void Scene05Bodies::Unload(Renderer& renderer) {
    sphere->Unload();
}