#ifndef MATRIX4X4_HPP
#define MATRIX4X4_HPP

#include "vecmat.hpp"

class Matrix4x4
{
	public:
		float f[4][4];

		Matrix4x4()
		{
			for(int y = 0; y < 4; y++)
				for(int x = 0; x < 4; x++)
					f[y][x] = 0.0;
		}

		Matrix4x4(Vector4 &a, Vector4 &b, Vector4 &c,  Vector4 &d)
		{
			f[0][0] = a.x; f[0][1] = a.y; f[0][2] = a.z; f[0][3] = a.w;
			f[1][0] = b.x; f[1][1] = b.y; f[1][2] = b.z; f[1][3] = b.w;
			f[2][0] = c.x; f[2][1] = c.y; f[2][2] = c.z; f[2][3] = c.w;
			f[3][0] = d.x; f[3][1] = d.y; f[3][2] = d.z; f[3][3] = d.w;
		}

		Matrix4x4 operator*(const Matrix4x4 &mat)
		{
			Matrix4x4 newMatrix;
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					for(int k = 0; k < 4; k++)
						newMatrix.f[j][i] += f[k][i] * mat.f[j][k];
			return newMatrix;
		}

		Vector3 operator*(const Vector3 &vec)
		{
			return Vector3(f[0][0] * vec.x + f[1][0] * vec.y + f[2][0] * vec.z + f[3][0],
						   f[0][1] * vec.x + f[1][1] * vec.y + f[2][1] * vec.z + f[3][1],
						   f[0][2] * vec.x + f[1][2] * vec.y + f[2][2] * vec.z + f[3][2]);
		}

		Vector2 operator*(const Vector2 &vec)
		{
			return Vector2(f[0][0] * vec.x + f[1][0] * vec.y + f[2][0],
						   f[0][1] * vec.x + f[1][1] * vec.y + f[2][1]);
		}

		static Matrix4x4 rotationMatrixX(const float a)
		{
			return Matrix4x4(Vector4(1, 0, 0, 0),
							 Vector4(0,  cos(a), sin(a), 0),
							 Vector4(0, -sin(a), cos(a), 0),
							 Vector4(0, 0, 0, 1));
		}

		static Matrix4x4 rotationMatrixY(const float a)
		{
			return Matrix4x4(Vector4(cos(a), 0, -sin(a), 0),
							 Vector4(0, 1, 0, 0),
							 Vector4(sin(a), 0, cos(a), 0),
							 Vector4(0, 0, 0, 1));
		}

		static Matrix4x4 rotationMatrixZ(const float a)
		{
			return Matrix4x4(Vector4( cos(a), sin(a), 0, 0),
							 Vector4(-sin(a), cos(a), 0, 0),
							 Vector4(0, 0, 1, 0),
							 Vector4(0, 0, 0, 1));
		}

		static Matrix4x4 translation(const Vector3 &vec)
		{
			return Matrix4x4(Vector4(1, 0, 0, 0),
							 Vector4(0, 1, 0, 0),
							 Vector4(0, 0, 1, 0),
							 Vector4(vec.x, vec.y, vec.z, 1));
		}

};

#endif