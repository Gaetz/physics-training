//
// Created by Gaëtan Blaise-Cazalet on 13/01/2025.
//

#include "Vec3.hpp"

namespace gmath {

    const Vec3 Vec3::zero { 0, 0, 0 };
    const Vec3 Vec3::one { 1, 1, 1 };
    const Vec3 Vec3::up { 0, 1, 0 };
    const Vec3 Vec3::right { 1, 0, 0 };
    const Vec3 Vec3::forward { 0, 0, -1 };
    const Vec3 Vec3::down { 0, -1, 0 };
    const Vec3 Vec3::left { -1, 0, 0 };
    const Vec3 Vec3::backward { 0, 0, 1 };
    
    void Vec3::Invert() {
        x = -x;
        y = -y;
        z = -z;
    }

    real Vec3::Magnitude() const {
        return RealSqrt(x * x + y * y + z * z);
    }

    real Vec3::SquareMagnitude() const {
        return x * x + y * y + z * z;
    }

    Vec3& Vec3::Normalize() {
        real length = Magnitude();
        if (length > 0) {
            (*this) *= ((real)1)/length;
        }
        return *this;
    }

    void Vec3::Clear() {
        x = y = z = 0;
    }

    void Vec3::operator*=(const real scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    Vec3 Vec3::operator*(const real scalar) const {
        return Vec3(x * scalar, y * scalar, z * scalar);
    }

    void Vec3::operator+=(const Vec3& v) {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    Vec3 Vec3::operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    void Vec3::operator-=(const Vec3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    Vec3 Vec3::operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    void Vec3::AddScaledVector(const Vec3& v, const real scale) {
        x += v.x * scale;
        y += v.y * scale;
        z += v.z * scale;
    }

    void Vec3::MakeOrthonormalBasis(Vec3& a, Vec3& b, Vec3& c) {
        a.Normalize();
        c = a % b;
        if (c.SquareMagnitude() == 0) {
            return;
        }
        c.Normalize();
        b = c % a;
    }

    Vec3 Vec3::ComponentProduct(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    void Vec3::ComponentProductUpdate(const Vec3& v) {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    void Vec3::operator%=(const Vec3& v) {
        *this = Cross(v);
    }
}
