//
// Created by gaetz on 03/02/2025.
//

#include "Body.hpp"
#include "Shape.hpp"
#include "Drawable.hpp"

namespace gphysics {

    Body::Body(const Vec &position_, const Quat &orientation_, Shape *shape_, Drawable *drawable_) :
            position{position_},
            orientation{orientation_},
            shape{shape_},
            drawable{drawable_} {

    }

    void Body::Update(f32 dt) {
        drawable->SetTransform(position, orientation, Vec::one);
        drawable->Update(dt);
    }

    Vec Body::GetCenterOfMassWorldSpace() const {
        const Vec centerOfMassBodySpace = shape->GetCenterOfMass();
        const Vec pos = position + orientation.RotatePoint(centerOfMassBodySpace);
        return pos;
    }

    Vec Body::GetCenterOfMassBodypace() const {
        return shape->GetCenterOfMass();
    }

    Vec Body::WorldSpaceToBodySpace(const Vec &worldPoint) {
        const Vec tmp = worldPoint - GetCenterOfMassWorldSpace();
        const Quat invertOrient = orientation.Inverse();
        Vec bodySpace = invertOrient.RotatePoint(tmp);
        return bodySpace;
    }

    Vec Body::BodySpaceToWorldSpace(const Vec &worldPoint) {
        return GetCenterOfMassWorldSpace() + orientation.RotatePoint(worldPoint);
    }

    void Body::ApplyImpulseLinear(const Vec &impulse) {
        if (inverseMass == 0) {
            return;
        }

        linearVelocity += impulse * inverseMass;
    }

}