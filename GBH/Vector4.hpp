#ifndef VECTOR4_HPP
#define VECTOR4_HPP

#include "vecmat.hpp"

class Vector4
{
	public:
		float x, y, z, w;

		Vector4()
			: x(0.0f), y(0.0f), z(0.0f), w(0.0f)
		{ }

		Vector4(float x, float y, float z, float w)
			: x(x), y(y), z(z), w(w)
		{ }

		Vector4(Vector2 &vec, float z = 0.0f, float w = 0.0f)
			: x(vec.x), y(vec.y), z(z), w(w)
		{ }

		Vector4(Vector3 &vec, float w = 0.0f)
			: x(vec.x), y(vec.y), z(vec.z), w(w)
		{ }

		Vector4(const Vector4 &vec)
			: x(vec.x), y(vec.y), z(vec.z), w(vec.w)
		{ }

		Vector4 &operator+=(const Vector4 &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			w += vec.w;
			return *this;
		}

		Vector4 &operator-=(const Vector4 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			w -= vec.w;
			return *this;
		}

		Vector4 &operator*=(const Vector4 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			w *= vec.w;
			return *this;
		}

		Vector4 &operator/=(const Vector4 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			w /= vec.w;
			return *this;
		}

		Vector4 &operator*=(float val)
		{
			x *= val;
			y *= val;
			z *= val;
			w *= val;
			return *this;
		}

		Vector4 &operator/=(float val)
		{
			x /= val;
			y /= val;
			z /= val;
			w /= val;
			return *this;
		}		
		
		Vector4 operator+(const Vector4 &vec)
		{
			return Vector4(x + vec.x, y + vec.y, z + vec.z, w + vec.w);
		}

		Vector4 operator-(const Vector4 &vec)
		{
			return Vector4(x - vec.x, y - vec.y, z - vec.z, w - vec.w);
		}

		Vector4 operator*(const Vector4 &vec)
		{
			return Vector4(x * vec.x, y * vec.y, z * vec.z, w * vec.w);
		}

		Vector4 operator/(const Vector4 &vec)
		{
			return Vector4(x / vec.x, y / vec.y, z / vec.z, w / vec.w);
		}

		float sum()
		{
			return x + y + z +w ;
		}
};

#endif