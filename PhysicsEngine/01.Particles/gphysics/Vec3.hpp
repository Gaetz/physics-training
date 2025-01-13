//
// Created by Gaëtan Blaise-Cazalet on 13/01/2025.
//

#include "Precision.hpp"

#ifndef GPHYSICS_VEC3_HPP
#define GPHYSICS_VEC3_HPP

namespace gphysics {
    class Vec3 {
    public:
        real x;
        real y;
        real z;
    private:
        real pad;

    public:
        Vec3() : x(0), y(0), z(0), pad(0) {}
        Vec3(real x, real y, real z) : x(x), y(y), z(z), pad(0) {}

        void Invert();
        real Magnitude();
        real SquareMagnitude();
        void Normalize();
    };
}


#endif //GPHYSICS_VEC3_HPP
