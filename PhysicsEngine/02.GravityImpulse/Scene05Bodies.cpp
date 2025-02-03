#include "Scene05Bodies.hpp"
#include "SphereCube.hpp"
#include "ShapeSphere.hpp"

using gphysics::ShapeSphere;


void Scene05Bodies::Load(Renderer& renderer) {
    auto shape = new ShapeSphere(1.0f);
    auto sphere = new SphereCube(renderer);
    sphere->Load();

    bodies.emplace_back(Vec::zero, Quat::identity, shape, sphere);
}

bool Scene05Bodies::Update(float dt) {
    bool stillRunning = ManageInput(inputState);
    if (inputState.IsPressed(DirectionalKey::Down)) {
        isPaused = !isPaused;
    }

    if (!isPaused) {
        for (auto &body: bodies) {
            body.linearVelocity += Vec{0.0f, -10.0f, 0.0f} * dt;
        }

        for (auto &body: bodies) {
            body.position += body.linearVelocity * dt;
            body.Update(dt);
        }
    }
    return stillRunning;
}

void Scene05Bodies::Draw(Renderer& renderer) {
    renderer.Begin();
    for (auto& body : bodies) {
        body.drawable->Draw(renderer);
    }
    renderer.End();
}

void Scene05Bodies::Unload(Renderer& renderer) {
    for (auto& body : bodies) {
        body.drawable->Unload();
    }
}