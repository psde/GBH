#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include "vecmat.hpp"

class Matrix3x3
{
	public:
		float f[3][3];

		Matrix3x3()
		{
			for(int y = 0; y < 3; y++)
				for(int x = 0; x < 3; x++)
					f[y][x] = 0.0;
		}

		Matrix3x3(Vector3 &a, Vector3 &b, Vector3 &c)
		{
			f[0][0] = a.x; f[0][1] = a.y; f[0][2] = a.z;
			f[1][0] = b.x; f[1][1] = b.y; f[1][2] = b.z;
			f[2][0] = c.x; f[2][1] = c.y; f[2][2] = c.z;
		}

		Matrix3x3 operator*(const Matrix3x3 &mat)
		{
			Matrix3x3 newMatrix;
			for(int i = 0; i < 3; i++)
				for(int j = 0; j < 3; j++)
					for(int k = 0; k < 3; k++)
						newMatrix.f[j][i] += f[k][i] * mat.f[j][k];
			return newMatrix;
		}

		Vector3 operator*(const Vector3 &vec)
		{
			return Vector3(f[0][0] * vec.x + f[1][0] * vec.x + f[2][0] * vec.x,
						   f[0][1] * vec.y + f[1][1] * vec.y + f[2][1] * vec.y,
						   f[0][2] * vec.z + f[1][2] * vec.z + f[2][2] * vec.z);
		}
};

#endif