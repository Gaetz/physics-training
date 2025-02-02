//
// Created by gaetz on 02/02/2025.
//

#ifndef GMATH_MAT3_HPP
#define GMATH_MAT3_HPP

#include <Defines.hpp>

namespace gmath {

    class Mat2;
    class Vec3;

    class Mat3 {
    public:
        static Mat3 zero;
        static Mat3 identity;

        Mat3() {}

        Mat3(const Mat3 &rhs);

        Mat3(const Vec3& row0_, const Vec3& row1_, const Vec3& row2_);

        Mat3(f32 m0_, f32 m3_, f32 m6_,
             f32 m1_, f32 m4_, f32 m7_,
             f32 m2_, f32 m5_, f32 m8_);

        [[nodiscard]] const f32 *ToArrayConst() const;
        [[nodiscard]] f32 *ToArray();

        Vec3 GetRow(i32 i) const;
        void SetRow(i32 i, const Vec3& newRow)

        void Zero();

        void Identity();

        float Trace() const;

        float Determinant() const;

        Mat3 Transpose() const;

        Mat3 Inverse() const;

        Mat2 Minor(const int i, const int j) const;

        float Cofactor(const int i, const int j) const;

        Vec3 operator*(const Vec3 &rhs) const;

        Mat3 operator*(const float rhs) const;

        Mat3 operator*(const Mat3 &rhs) const;

        Mat3 operator+(const Mat3 &rhs) const;

        const Mat3 &operator*=(const float rhs);

        const Mat3 &operator+=(const Mat3 &rhs);

    public:
        f32 m0, m3, m6;
        f32 m1, m4, m7;
        f32 m2, m5, m8;
    };

}


#endif //GMATH_MAT3_HPP
