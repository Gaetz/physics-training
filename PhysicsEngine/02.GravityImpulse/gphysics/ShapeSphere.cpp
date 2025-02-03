//
// Created by gaetz on 03/02/2025.
//

#include "ShapeSphere.hpp"
#include "Vec.hpp"

gphysics::ShapeSphere::ShapeSphere(float radiusP) : radius(radiusP) {
    centerOfMass = Vec::zero;
}
