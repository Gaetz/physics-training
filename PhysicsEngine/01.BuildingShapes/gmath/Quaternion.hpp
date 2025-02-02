//
// Created by gaetz on 02/02/2025.
//

#ifndef GMATH_QUATERNION_HPP
#define GMATH_QUATERNION_HPP

#include "Vec3.hpp"
#include "Core.hpp"

namespace gmath {

    class Quaternion {
    public:
        real x;
        real y;
        real z;
        real w;

        Quaternion() {
            *this = Quaternion::Identity;
        }

        // This directly Sets the quaternion components --
        // don't use for axis/angle
        explicit Quaternion(real inX, real inY, real inZ, real inW) {
            Set(inX, inY, inZ, inW);
        }

        // Construct the quaternion from an axis and angle
        // It is assumed that axis is already Normalized,
        // and the angle is in radians
        explicit Quaternion(const Vec3 &axis, real angle) {
            real scalar = RealSin(angle / 2.0f);
            x = axis.x * scalar;
            y = axis.y * scalar;
            z = axis.z * scalar;
            w = RealCos(angle / 2.0f);
        }

        // Directly Set the internal components
        void Set(real inX, real inY, real inZ, real inW) {
            x = inX;
            y = inY;
            z = inZ;
            w = inW;
        }

        void Conjugate() {
            x *= -1.0f;
            y *= -1.0f;
            z *= -1.0f;
        }

        real LengthSq() const {
            return (x * x + y * y + z * z + w * w);
        }

        real Length() const {
            return RealSqrt(LengthSq());
        }

        void Normalize() {
            real len = Length();
            x /= len;
            y /= len;
            z /= len;
            w /= len;
        }

        // Normalize the provided quaternion
        static Quaternion Normalize(const Quaternion &q) {
            Quaternion retVal = q;
            retVal.Normalize();
            return retVal;
        }

        // Linear interpolation
        static Quaternion Lerp(const Quaternion &a, const Quaternion &b, real f) {
            Quaternion retVal;
            retVal.x = gmath::Lerp(a.x, b.x, f);
            retVal.y = gmath::Lerp(a.y, b.y, f);
            retVal.z = gmath::Lerp(a.z, b.z, f);
            retVal.w = gmath::Lerp(a.w, b.w, f);
            retVal.Normalize();
            return retVal;
        }

        static real Dot(const Quaternion &a, const Quaternion &b) {
            return a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
        }

        // Spherical Linear Interpolation
        static Quaternion Slerp(const Quaternion &a, const Quaternion &b, real f) {
            real rawCosm = Quaternion::Dot(a, b);

            real cosom = -rawCosm;
            if (rawCosm >= 0.0f) {
                cosom = rawCosm;
            }

            real scale0, scale1;

            if (cosom < 0.9999f) {
                const real omega = acos(cosom);
                const real invSin = 1.f / RealSin(omega);
                scale0 = RealSin((1.f - f) * omega) * invSin;
                scale1 = RealSin(f * omega) * invSin;
            } else {
                // Use linear interpolation if the quaternions
                // are collinear
                scale0 = 1.0f - f;
                scale1 = f;
            }

            if (rawCosm < 0.0f) {
                scale1 = -scale1;
            }

            Quaternion retVal;
            retVal.x = scale0 * a.x + scale1 * b.x;
            retVal.y = scale0 * a.y + scale1 * b.y;
            retVal.z = scale0 * a.z + scale1 * b.z;
            retVal.w = scale0 * a.w + scale1 * b.w;
            retVal.Normalize();
            return retVal;
        }

        // Concatenate
        // Rotate by q FOLLOWED BY p
        static Quaternion Concatenate(const Quaternion &q, const Quaternion &p) {
            Quaternion retVal;

            // Vector component is:
            // ps * qv + qs * pv + pv x qv
            Vec3 qv(q.x, q.y, q.z);
            Vec3 pv(p.x, p.y, p.z);
            Vec3 newVec = p.w * qv + q.w * pv + pv.Cross(qv);
            retVal.x = newVec.x;
            retVal.y = newVec.y;
            retVal.z = newVec.z;

            // Scalar component is:
            // ps * qs - pv . qv
            retVal.w = p.w * q.w - pv.Dot(qv);

            return retVal;
        }

        static const Quaternion Identity;
    };
}

#endif //GMATH_QUATERNION_HPP
