#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "vecmat.hpp"

class Vector3
{
	public:
		float x, y, z;

		Vector3()
			: x(0.0f), y(0.0f), z(0.0f)
		{ }

		Vector3(Vector2 &vec)
			: x(vec.x), y(vec.y), z(0.0f)
		{ }

		Vector3(Vector2 &vec, float z)
			: x(vec.x), y(vec.y), z(z)
		{ }

		Vector3(float x, float y, float z)
			: x(x), y(y), z(z)
		{ }

		Vector3(Vector3 &vec)
			: x(vec.x), y(vec.y), z(vec.z)
		{ }

		Vector3 &operator+=(const Vector3 &vec)
		{
			x += vec.x;
			y += vec.y;
			z += vec.z;
			return *this;
		}

		Vector3 &operator-=(const Vector3 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;
			return *this;
		}

		Vector3 &operator*=(const Vector3 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			z *= vec.z;
			return *this;
		}

		Vector3 &operator/=(const Vector3 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			z /= vec.z;
			return *this;
		}

		Vector3 &operator*=(float val)
		{
			x *= val;
			y *= val;
			z *= val;
			return *this;
		}

		Vector3 &operator/=(float val)
		{
			x /= val;
			y /= val;
			z /= val;
			return *this;
		}		
		
		Vector3 operator+(const Vector3 &vec)
		{
			return Vector3(x + vec.x, y + vec.y, z + vec.z);
		}

		Vector3 operator-(const Vector3 &vec)
		{
			return Vector3(x - vec.x, y - vec.y, z - vec.z);
		}

		Vector3 operator*(const Vector3 &vec)
		{
			return Vector3(x * vec.x, y * vec.y, z * vec.z);
		}

		Vector3 operator/(const Vector3 &vec)
		{
			return Vector3(x / vec.x, y / vec.y, z / vec.z);
		}

		float sum()
		{
			return x + y + z;
		}

		float dot(const Vector3 &vec)
		{
			return Vector3(*this * vec).sum(); //return x * vec.x + y * vec.y + z * vec.z;
		}

		Vector3 cross(Vector3 &vec)
		{	
			return Vector3(	y * vec.z - z * vec.y,
							z * vec.x - x * vec.z,
							x * vec.y - y * vec.x);
		}

};

#endif