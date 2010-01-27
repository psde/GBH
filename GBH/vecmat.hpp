#ifndef VECMAT_HPP
#define VECMAT_HPP

#include "math.h"

namespace Math {
	const float PI = 3.14159265358979;
	inline float degreesToRadians(float degrees)
	{
		return degrees * Math::PI / 180.0;
	}

	inline float radiansToDegrees(float radians)
	{
		return radians * 180.0 / Math::PI;
	}
}

#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "matrix3x3.hpp"
#include "matrix4x4.hpp"

#endif