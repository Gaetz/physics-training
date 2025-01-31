#pragma once

#ifndef GMATH_MAT4_HPP
#define GMATH_MAT4_HPP

#include <Defines.hpp>

namespace gmath {
class Mat4 
{
public:
	Mat4();
	Mat4(f32 m0_, f32 m4_, f32 m8_, f32 m12_,
		f32 m1_, f32 m5_, f32 m9_, f32 m13_,
		f32 m2_, f32 m6_, f32 m10_, f32 m14_,
		f32 m3_, f32 m7_, f32 m11_, f32 m15_) 
		:
		m0{ m0_ }, m4{ m4_ }, m8{ m8_ }, m12{ m12_ },
		m1{ m1_ }, m5{ m5_ }, m9{ m9_ }, m13{ m13_ },
		m2{ m2_ }, m6{ m6_ }, m10{ m10_ }, m14{ m14_ },
		m3{ m3_ }, m7{ m7_ }, m11{ m11_ }, m15{ m15_ } {}

	f32 m0, m4, m8, m12; // Matrix first row (4 components)
	f32 m1, m5, m9, m13; // Matrix second row (4 components)
	f32 m2, m6, m10, m14; // Matrix third row (4 components)
	f32 m3, m7, m11, m15; // Matrix fourth row (4 components)

	static const Mat4 Identity;

	static Mat4 CreateRotationZ(f32 angle);
	static Mat4 CreateRotationX(f32 angle);
	static Mat4 CreateRotationY(f32 angle);
	static Mat4 CreateTranslation(f32 x, f32 y, f32 z);

    [[nodiscard]] const f32* ToArray() const;

	Mat4 operator*(const Mat4& other) const;

	static Mat4 CreateOrthographicOffCenter(f32 left, f32 right, f32 bottom, f32 top, f32 zNearPlane, f32 zFarPlane);

	static Mat4 CreatePerspectiveFieldOfView(f32 fieldOfView, f32 aspectRatio, f32 nearPlaneDistance, f32 farPlaneDistance);
};
}

#endif //GMATH_MAT4_HPP