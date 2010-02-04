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

		Matrix4x4(Vector2 &a, Vector2 &b, Vector2 &c,  Vector2 &d)
		{
			f[0][0] = a.x; f[0][1] = a.y; f[0][2] = 0; f[0][3] = 0;
			f[1][0] = b.x; f[1][1] = b.y; f[1][2] = 0; f[1][3] = 0;
			f[2][0] = c.x; f[2][1] = c.y; f[2][2] = 1; f[2][3] = 0;
			f[3][0] = d.x; f[3][1] = d.y; f[3][2] = 0; f[3][3] = 1;
		}

		Matrix4x4(Vector3 &a, Vector3 &b, Vector3 &c,  Vector3 &d)
		{
			f[0][0] = a.x; f[0][1] = a.y; f[0][2] = a.z; f[0][3] = 0;
			f[1][0] = b.x; f[1][1] = b.y; f[1][2] = b.z; f[1][3] = 0;
			f[2][0] = c.x; f[2][1] = c.y; f[2][2] = c.z; f[2][3] = 0;
			f[3][0] = d.x; f[3][1] = d.y; f[3][2] = d.z; f[3][3] = 1;
		}

		Matrix4x4(Vector4 &a, Vector4 &b, Vector4 &c,  Vector4 &d)
		{
			f[0][0] = a.x; f[0][1] = a.y; f[0][2] = a.z; f[0][3] = a.w;
			f[1][0] = b.x; f[1][1] = b.y; f[1][2] = b.z; f[1][3] = b.w;
			f[2][0] = c.x; f[2][1] = c.y; f[2][2] = c.z; f[2][3] = c.w;
			f[3][0] = d.x; f[3][1] = d.y; f[3][2] = d.z; f[3][3] = d.w;
		}

		Matrix4x4(float e0, float e1, float e2, float e3,
				  float e4, float e5, float e6, float e7,
				  float e8, float e9, float e10, float e11,
				  float e12, float e13, float e14, float e15)
		{
			f[0][0] = e0; f[0][1] = e1; f[0][2] = e2; f[0][3] = e3;
			f[1][0] = e4; f[1][1] = e5; f[1][2] = e6; f[1][3] = e7;
			f[2][0] = e8; f[2][1] = e9; f[2][2] = e10; f[2][3] = e11;
			f[3][0] = e12; f[3][1] = e13; f[3][2] = e14; f[3][3] = 15;
		}

		Matrix4x4(float e[16])
		{
			f[0][0] = e[0]; f[0][1] = e[1]; f[0][2] = e[2]; f[0][3] = e[3];
			f[1][0] = e[4]; f[1][1] = e[5]; f[1][2] = e[6]; f[1][3] = e[7];
			f[2][0] = e[8]; f[2][1] = e[9]; f[2][2] = e[10]; f[2][3] = e[11];
			f[3][0] = e[12]; f[3][1] = e[13]; f[3][2] = e[14]; f[3][3] = e[15];
		}

		Matrix4x4 operator*(const Matrix4x4 &mat) const
		{
			Matrix4x4 newMatrix;
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					for(int k = 0; k < 4; k++)
						newMatrix.f[j][i] += f[k][i] * mat.f[j][k];
			return newMatrix;
		}

		Vector4 operator*(const Vector4 &vec) const
		{
			return Vector4(f[0][0] * vec.x + f[1][0] * vec.y + f[2][0] * vec.z + f[3][0] * vec.w,
						   f[0][1] * vec.x + f[1][1] * vec.y + f[2][1] * vec.z + f[3][1] * vec.w,
						   f[0][2] * vec.x + f[1][2] * vec.y + f[2][2] * vec.z + f[3][2] * vec.w,
						   f[0][3] * vec.x + f[1][3] * vec.y + f[2][3] * vec.z + f[3][3] * vec.w);
		}

		Vector3 operator*(const Vector3 &vec) const
		{
			return Vector3(f[0][0] * vec.x + f[1][0] * vec.y + f[2][0] * vec.z + f[3][0],
						   f[0][1] * vec.x + f[1][1] * vec.y + f[2][1] * vec.z + f[3][1],
						   f[0][2] * vec.x + f[1][2] * vec.y + f[2][2] * vec.z + f[3][2]);
		}

		Vector2 operator*(const Vector2 &vec) const
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

		static Matrix4x4 scale(const Vector3 &vec)
		{
			return Matrix4x4(Vector4(vec.x, 0, 0, 0),
							 Vector4(0, vec.y, 0, 0),
							 Vector4(0, 0, vec.z, 0),
							 Vector4(0, 0, 0, 1));
		}

		Matrix4x4& Matrix4x4::setIdentity()
		{
			for(int i = 0; i < 4; i++)
				for(int j = 0; j < 4; j++)
					f[i][j] = (i == j) ? 1 : 0;
			return *this;
		}

		Matrix4x4 transpose()
		{
			return Matrix4x4(f[0][0], f[0][1], f[0][2], f[0][3],
							 f[1][0], f[1][1], f[1][2], f[1][3],
							 f[2][0], f[2][1], f[2][2], f[2][3],
							 f[3][0], f[3][1], f[3][2], f[3][3]);
		}
};

#endif