//
// Created by Gaëtan Blaise-Cazalet on 19/11/2024.
//

#ifndef SCENE01CLEAR_HPP
#define SCENE01CLEAR_HPP

#include <Quad.hpp>

#include "Scene.hpp"

using gdraw::Quad;

class Scene01Clear : public Scene {
public:
    void Load(Renderer& renderer) override;
    bool Update(float dt) override;
    void Draw(Renderer& renderer) override;
    void Unload(Renderer& renderer) override;

private:
    uptr<Quad> quad;
    InputState inputState;
};



#endif //SCENE01CLEAR_HPP