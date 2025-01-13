//
// Created by Gaëtan Blaise-Cazalet on 13/01/2025.
//

#include "Vec3.hpp"

namespace gphysics {
    void Vec3::Invert() {
        x = -x;
        y = -y;
        z = -z;
    }

    real Vec3::Magnitude() {
        return RealSqrt(x * x + y * y + z * z);
    }

    real Vec3::SquareMagnitude() {
        return x * x + y * y + z * z;
    }

    void Vec3::Normalize() {
        real length = Magnitude();
        if (length > 0) {
            const real invLength = static_cast<real>(1) / length;
            x *= invLength;
            y *= invLength;
            z *= invLength;
        }
    }
}