#include "Mat4.hpp"
#include <cstdlib>

#include <Vec3.hpp>
#include <Quat.hpp>

namespace gmath {
    const Mat4 Mat4::identity
            {
                    1.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 1.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 1.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 1.0f
            };

    const Mat4 Mat4::zero
            {
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f,
                    0.0f, 0.0f, 0.0f, 0.0f
            };

    Mat4 Mat4::CreateRotationZ(float radians) {
        return Mat4
                {
                        cosf(radians), sinf(radians), 0, 0,
                        -sinf(radians), cosf(radians), 0, 0,
                        0, 0, 1, 0,
                        0, 0, 0, 1
                };
    }

    Mat4 Mat4::CreateRotationX(float radians) {
        return Mat4
                {
                        1, 0, 0, 0,
                        0, cosf(radians), 0, sinf(radians),
                        0, -sinf(radians), 0, cosf(radians),
                        0, 0, 0, 1
                };
    }

    Mat4 Mat4::CreateRotationY(float radians) {
        return Mat4
                {
                        cosf(radians), sinf(radians), 0, 0,
                        0, 1, 0, 0,
                        -sinf(radians), cosf(radians), 0, 0,
                        0, 0, 0, 1
                };
    }

    Mat4 Mat4::CreateTranslation(float x, float y, float z) {
        return Mat4
                {
                        1, 0, 0, 0,
                        0, 1, 0, 0,
                        0, 0, 1, 0,
                        x, y, z, 1
                };
    }

    // Extract the scale component from the matrix
    Vec3 Mat4::GetScale() const {
        Vec3 retVal;
        retVal.x = Vec3(m0, m1, m2).Magnitude();
        retVal.y = Vec3(m4, m5, m3).Magnitude();
        retVal.z = Vec3(m8, m9, m10).Magnitude();
        return retVal;
    }

    // Create a scale matrix with x, y, and z scales
    Mat4 Mat4::CreateScale(float xScale, float yScale, float zScale) {
        return
                {
                        xScale, 0.0f, 0.0f, 0.0f,
                        0.0f, yScale, 0.0f, 0.0f,
                        0.0f, 0.0f, zScale, 0.0f,
                        0.0f, 0.0f, 0.0f, 1.0f
                };
    }

    Mat4 Mat4::CreateScale(const Vec3 &scaleVector) {
        return CreateScale(scaleVector.x, scaleVector.y, scaleVector.z);
    }

    // Create a scale matrix with a uniform factor
    Mat4 Mat4::CreateScale(float scale) {
        return CreateScale(scale, scale, scale);
    }

    Mat4 Mat4::CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane,
                                           float zFarPlane) {
        return Mat4
                {
                        2.0f / (right - left), 0, 0, 0,
                        0, 2.0f / (top - bottom), 0, 0,
                        0, 0, 1.0f / (zNearPlane - zFarPlane), 0,
                        (left + right) / (left - right), (top + bottom) / (bottom - top),
                        zNearPlane / (zNearPlane - zFarPlane), 1
                };
    }

    Mat4 Mat4::CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float nearPlaneDistance,
                                            float farPlaneDistance) {
        float num = 1.0f / ((float) tanf(fieldOfView * 0.5f));

        return Mat4
                {
                        num / aspectRatio, 0, 0, 0,
                        0, num, 0, 0,
                        0, 0, farPlaneDistance / (nearPlaneDistance - farPlaneDistance), -1,
                        0, 0, (nearPlaneDistance * farPlaneDistance) / (nearPlaneDistance - farPlaneDistance), 0
                };
    }

    Mat4 Mat4::operator*(const Mat4 &other) const {
        Mat4 result;

        result.m0 = (
                (m0 * other.m0) +
                (m4 * other.m1) +
                (m8 * other.m2) +
                (m12 * other.m3)
        );

        result.m4 = (
                (m0 * other.m4) +
                (m4 * other.m5) +
                (m8 * other.m6) +
                (m12 * other.m7)
        );

        result.m8 = (
                (m0 * other.m8) +
                (m4 * other.m9) +
                (m8 * other.m10) +
                (m12 * other.m11)
        );

        result.m12 = (
                (m0 * other.m12) +
                (m4 * other.m13) +
                (m8 * other.m14) +
                (m12 * other.m15)
        );

        result.m1 = (
                (m1 * other.m0) +
                (m5 * other.m1) +
                (m9 * other.m2) +
                (m13 * other.m3)
        );

        result.m5 = (
                (m1 * other.m4) +
                (m5 * other.m5) +
                (m9 * other.m6) +
                (m13 * other.m7)
        );

        result.m9 = (
                (m1 * other.m8) +
                (m5 * other.m9) +
                (m9 * other.m10) +
                (m13 * other.m11)
        );

        result.m13 = (
                (m1 * other.m12) +
                (m5 * other.m13) +
                (m9 * other.m14) +
                (m13 * other.m15)
        );

        result.m2 = (
                (m2 * other.m0) +
                (m6 * other.m1) +
                (m10 * other.m2) +
                (m14 * other.m3)
        );

        result.m6 = (
                (m2 * other.m4) +
                (m6 * other.m5) +
                (m10 * other.m6) +
                (m14 * other.m7)
        );

        result.m10 = (
                (m2 * other.m8) +
                (m6 * other.m9) +
                (m10 * other.m10) +
                (m14 * other.m11)
        );

        result.m14 = (
                (m2 * other.m12) +
                (m6 * other.m13) +
                (m10 * other.m14) +
                (m14 * other.m15)
        );

        result.m3 = (
                (m3 * other.m0) +
                (m7 * other.m1) +
                (m11 * other.m2) +
                (m15 * other.m3)
        );

        result.m7 = (
                (m3 * other.m4) +
                (m7 * other.m5) +
                (m11 * other.m6) +
                (m15 * other.m7)
        );

        result.m11 = (
                (m3 * other.m8) +
                (m7 * other.m9) +
                (m11 * other.m10) +
                (m15 * other.m11)
        );

        result.m15 = (
                (m3 * other.m12) +
                (m7 * other.m13) +
                (m11 * other.m14) +
                (m15 * other.m15)
        );

        return result;
    }

    Mat4::Mat4() {
        m0 = 0.0f;
        m1 = 0.0f;
        m2 = 0.0f;
        m3 = 0.0f;

        m4 = 0.0f;
        m5 = 0.0f;
        m6 = 0.0f;
        m7 = 0.0f;

        m8 = 0.0f;
        m9 = 0.0f;
        m10 = 0.0f;
        m0 = 0.0f;

        m4 = 0.0f;
        m8 = 0.0f;
        m12 = 0.0f;
        m15 = 0.0f;
    }

    const f32 *Mat4::ToArray() const {
        return reinterpret_cast<const f32 *>(this);
    }

    Mat4 Mat4::CreateLookAt(const Vec3 &eye, const Vec3 &target, const Vec3 &up) {
        Vec3 zAxis = (target - eye).Normalize();
        Vec3 xAxis = up.Cross(zAxis).Normalize();
        Vec3 yAxis = zAxis.Cross(xAxis);

        return Mat4{
                xAxis.x, yAxis.x, zAxis.x, 0,
                xAxis.y, yAxis.y, zAxis.y, 0,
                xAxis.z, yAxis.z, zAxis.z, 0,
                -xAxis.Dot(eye), -yAxis.Dot(eye), -zAxis.Dot(eye), 1
        };
    }

    Mat4 Mat4::CreateFromQuaternion(const Quat &q) {
        Mat4 mat;

        mat.m0 = 1.0f - 2.0f * q.y * q.y - 2.0f * q.z * q.z;
        mat.m4 = 2.0f * q.x * q.y + 2.0f * q.w * q.z;
        mat.m8 = 2.0f * q.x * q.z - 2.0f * q.w * q.y;
        mat.m12 = 0.0f;

        mat.m1 = 2.0f * q.x * q.y - 2.0f * q.w * q.z;
        mat.m5 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.z * q.z;
        mat.m9 = 2.0f * q.y * q.z + 2.0f * q.w * q.x;
        mat.m13 = 0.0f;

        mat.m2 = 2.0f * q.x * q.z + 2.0f * q.w * q.y;
        mat.m6 = 2.0f * q.y * q.z - 2.0f * q.w * q.x;
        mat.m10 = 1.0f - 2.0f * q.x * q.x - 2.0f * q.y * q.y;
        mat.m14 = 0.0f;

        mat.m3 = 0.0f;
        mat.m7 = 0.0f;
        mat.m11 = 0.0f;
        mat.m15 = 1.0f;

        return mat;
    }


    float Mat4::Trace() const {
        const float xx = m0 * m0;
        const float yy = m5 * m5;
        const float zz = m10 * m10;
        const float ww = m15 * m15;
        return ( xx + yy + zz + ww );
    }

    inline float Mat4::Determinant() const {
        float det = 0.0f;
        float sign = 1.0f;
        for ( int j = 0; j < 4; j++ ) {
            Mat3 minor = Minor( 0, j );

            det += rows[ 0 ][ j ] * minor.Determinant() * sign;
            sign = sign * -1.0f;
        }
        return det;
    }

    inline Mat4 Mat4::Transpose() const {
        Mat4 transpose;
        for ( int i = 0; i < 4; i++ ) {
            for ( int j = 0; j < 4; j++ ) {
                transpose.rows[ i ][ j ] = rows[ j ][ i ];
            }
        }
        return transpose;
    }

    inline Mat4 Mat4::Inverse() const {
        Mat4 inv;
        for ( int i = 0; i < 4; i++ ) {
            for ( int j = 0; j < 4; j++ ) {
                inv.rows[ j ][ i ] = Cofactor( i, j );	// Perform the transpose while calculating the cofactors
            }
        }
        float det = Determinant();
        float invDet = 1.0f / det;
        inv *= invDet;
        return inv;
    }

    inline Mat3 Mat4::Minor( const int i, const int j ) const {
        Mat3 minor;

        int yy = 0;
        for ( int y = 0; y < 4; y++ ) {
            if ( y == j ) {
                continue;
            }

            int xx = 0;
            for ( int x = 0; x < 4; x++ ) {
                if ( x == i ) {
                    continue;
                }

                minor.rows[ xx ][ yy ] = rows[ x ][ y ];
                xx++;
            }

            yy++;
        }
        return minor;
    }

    inline float Mat4::Cofactor( const int i, const int j ) const {
        const Mat3 minor = Minor( i, j );
        const float C = float( pow( -1, i + 1 + j + 1 ) ) * minor.Determinant();
        return C;
    }

    inline void Mat4::Orient( Vec3 pos, Vec3 fwd, Vec3 up ) {
        Vec3 left = up.Cross( fwd );

        // For our coordinate system where:
        // +x-axis = fwd
        // +y-axis = left
        // +z-axis = up
        rows[ 0 ] = Vec4( fwd.x, left.x, up.x, pos.x );
        rows[ 1 ] = Vec4( fwd.y, left.y, up.y, pos.y );
        rows[ 2 ] = Vec4( fwd.z, left.z, up.z, pos.z );
        rows[ 3 ] = Vec4( 0, 0, 0, 1 );
    }
}