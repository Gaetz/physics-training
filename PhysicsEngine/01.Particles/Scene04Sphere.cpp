#include "Scene04Sphere.hpp"

void Scene04Sphere::Load(Renderer& renderer) {
    cube = std::make_unique<Cube>(renderer);
    cube->Load();
}

bool Scene04Sphere::Update(float dt) {
    bool stillRunning = ManageInput(inputState);
    cube->Update(dt);
    return stillRunning;
}

void Scene04Sphere::Draw(Renderer& renderer) {
    renderer.Begin();

    cube->Draw();

    renderer.End();
}

void Scene04Sphere::Unload(Renderer& renderer) {
    cube->Unload();
}