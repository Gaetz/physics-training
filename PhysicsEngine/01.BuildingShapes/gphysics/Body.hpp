//
// Created by gaetz on 03/02/2025.
//

#ifndef GPHYSICS_BODY_HPP
#define GPHYSICS_BODY_HPP

#include "Vec.hpp"
#include "Quat.hpp"

using gmath::Vec;
using gmath::Quat;
class Shape;

namespace gphysics {
    class Body
    {
    public:
        Vec position;
        Quat orientation;
        Shape* shape;
    };
}



#endif //GPHYSICS_BODY_HPP
