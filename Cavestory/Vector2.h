#ifndef VECTOR2_H_
#define VECTOR2_H_

#include <cmath>

struct Vector2
{
	Vector2(float x, float y);
	Vector2 operator+(const Vector2 &vector);
	Vector2 operator-(const Vector2 &vector);
	Vector2 operator*(const float &scalar);
	friend	float operator*(const Vector2 &vector, const Vector2 &vector2);
	Vector2 operator/(const float &scalar);

	float length();
	Vector2 normalize();


	float x;
	float y;
};

#endif
