//
// Created by Gaëtan Blaise-Cazalet on 13/01/2025.
//

#include "Precision.hpp"

#ifndef GMATH_VEC3_HPP
#define GMATH_VEC3_HPP

namespace gmath {
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

        static const Vec3 zero;
        static const Vec3 one;
        static const Vec3 up;
        static const Vec3 right;
        static const Vec3 forward;
        static const Vec3 down;
        static const Vec3 left;
        static const Vec3 backward;

        void Invert();
        [[nodiscard]] real Magnitude() const;
        [[nodiscard]] real SquareMagnitude() const;
        Vec3& Normalize();
        void Clear();

        void operator*=(real scalar);
        Vec3 operator*(real scalar) const;
        void operator+=(const Vec3& v);
        Vec3 operator+(const Vec3& v) const;
        void operator-=(const Vec3& v);
        Vec3 operator-(const Vec3& v) const;
        void AddScaledVector(const Vec3& v, real scale);

        /** Makes an orthonormal basis from vector, that won't change direction.
         * Vector b and c will be oriented so that the base is orthonormal.
         * All vectors will be normalized.
         * This function is working for a right-handed coordinate system.
         */
        static void MakeOrthonormalBasis(Vec3& a, Vec3& b, Vec3& c);

        /** Calculate and return a component-wise product of this vector with the given vector. */
        Vec3 ComponentProduct(const Vec3& v) const;

        /** Perform a component-wise product with the given vector and set this vector to its result. */
        void ComponentProductUpdate(const Vec3& v);

        /** Calculate and return the scalar product of this vector with the given vector. */
        real ScalarProduct(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

        /** Calculate and return the scalar product of this vector with the given vector. */
        real Dot(const Vec3& v) { return x * v.x + y * v.y + z * v.z; }

        /** Calculate and return the scalar product of this vector with the given vector. */
        real operator*(const Vec3& v) const { return x * v.x + y * v.y + z * v.z;}

        /** Calculate and return the vector product of this vector with the given vector. */
        Vec3 VectorProduct(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

        /** Calculate and return the vector product of this vector with the given vector. */
        Vec3 Cross(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

        /** Calculate and return the vector product of this vector with the given vector. */
        Vec3 operator%(const Vec3& v) const { return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x); }

        /** Updates this vector to be the vector product of its current value and the given vector. */
        void operator%=(const Vec3& v);
    };
}


#endif //GMATH_VEC3_HPP
