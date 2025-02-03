//
// Created by gaetz on 02/02/2025.
//

#ifndef GPHYSICS_SHAPE_HPP
#define GPHYSICS_SHAPE_HPP

#include "Vec.hpp"

using gmath::Vec;

namespace gphysics {
    class Shape {
    public:
        enum class ShapeType
        {
            SHAPE_SPHERE,
        };

        [[nodiscard]] virtual ShapeType GetType() const = 0;
        [[nodiscard]] Vec GetCenterOfMass() const { return centerOfMass; };

    protected:
        Vec centerOfMass {};
    };
}




#endif //GPHYSICS_SHAPE_HPP
