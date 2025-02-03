//
// Created by gaetz on 03/02/2025.
//

#ifndef GDRAW_DRAWABLE_HPP
#define GDRAW_DRAWABLE_HPP

#include <Defines.hpp>

namespace gmath {
    class Mat4;
    class Vec;
    class Quat;
}

using gmath::Mat4;
using gmath::Vec;
using gmath::Quat;

namespace gdraw {

    class Renderer;

    class Drawable {
    public:
        virtual void Load() = 0;
        virtual void Update(f32 dt) = 0;
        virtual void Draw(const Renderer& renderer) = 0;
        virtual void Unload() = 0;
        virtual void SetTransform(const Mat4& transform) = 0;
        virtual void SetTransform(const Vec& position, const Quat& rotation, const Vec& scale) = 0;

    };
}

#endif //GDRAW_DRAWABLE_HPP
