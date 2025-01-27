#include "Scene01Clear.hpp"

void Scene01Clear::Load(Renderer& renderer) {
    quad = std::make_unique<Quad>(renderer);
    quad->Load();
}

bool Scene01Clear::Update(float dt) {
    bool stillRunning = ManageInput(inputState);
    quad->Update(dt);
    return stillRunning;
}

void Scene01Clear::Draw(Renderer& renderer) {
    renderer.Begin();

    quad->Draw();

    renderer.End();
}

void Scene01Clear::Unload(Renderer& renderer) {
    quad->Unload();
}