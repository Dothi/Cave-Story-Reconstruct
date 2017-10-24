#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>

struct Vector2
{
	Vector2(float x, float y);
	Vector2();
	Vector2 operator+(const Vector2 &vector) const;
	Vector2 operator-(const Vector2 &vector) const;
	Vector2 operator*(const float &scalar) const;
	friend const float operator*(const Vector2 &vector, const Vector2 &vector2);
	Vector2 operator/(const float &scalar) const;

	float length() const;
	Vector2 normalize() const;


	float x;
	float y;
};

#endif
