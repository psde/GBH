#ifndef VECTOR2_HPP
#define VECTOR2_HPP

class Vector2
{
	public:
		float x, y;

		Vector2()
			: x(0.0f), y(0.0f)
		{ }

		Vector2(float x, float y)
			: x(x), y(y)
		{ }
		
		Vector2(Vector2 &vec)
			: x(vec.x), y(vec.y)
		{ }

		Vector2 &operator+=(const Vector2 &vec)
		{
			x += vec.x;
			y += vec.y;
			return *this;
		}

		Vector2 &operator-=(const Vector2 &vec)
		{
			x -= vec.x;
			y -= vec.y;
			return *this;
		}

		Vector2 &operator*=(const Vector2 &vec)
		{
			x *= vec.x;
			y *= vec.y;
			return *this;
		}

		Vector2 &operator/=(const Vector2 &vec)
		{
			x /= vec.x;
			y /= vec.y;
			return *this;
		}

		Vector2& operator*=(float val)
		{
			x *= val;
			y *= val;
			return *this;
		}

		Vector2& operator/=(float val)
		{
			x /= val;
			y /= val;
			return *this;
		}

		Vector2 operator+(const Vector2 &vec)
		{
			return Vector2(x + vec.x, y + vec.y);
		}		
		
		Vector2 operator-(const Vector2 &vec)
		{
			return Vector2(x - vec.x, y - vec.y);
		}
		
		Vector2 operator*(const Vector2 &vec)
		{
			return Vector2(x * vec.x, y * vec.y);
		}
		
		Vector2 operator/(const Vector2 &vec)
		{
			return Vector2(x / vec.x, y / vec.y);
		}
};

#endif