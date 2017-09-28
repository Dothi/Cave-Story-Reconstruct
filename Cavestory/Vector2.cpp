#include "Vector2.h"


Vector2::Vector2(float _x, float _y) :
	x(_x), y(_y)
{

}

Vector2 Vector2::operator+(const Vector2 &vector)
{
	Vector2 result = Vector2(0.0f, 0.0f);

	result.x = x + vector.x;
	result.y = y + vector.y;

	return result;
}

Vector2 Vector2::operator-(const Vector2 &vector)
{
	Vector2 result = Vector2(0.0f, 0.0f);

	result.x = x - vector.x;
	result.y = y - vector.y;

	return result;
}

Vector2 Vector2::operator*(const float &scalar)
{

	Vector2 result = Vector2(0.0f, 0.0f);

	result.x = x * scalar;
	result.y = y * scalar;

	return result;
}

float operator*(const Vector2 &vector, const Vector2 &vector2)
{
	float result = 0.0f;

	result = vector.x * vector2.x + vector.y * vector2.y;

	return result;
}

Vector2 Vector2::operator/(const float &scalar)
{
	Vector2 result(x, y);

	result = result * (1.0f / scalar);

	return result;
}

float Vector2::length()
{
	float length;

	length = sqrt((pow(x, 2.0f) + pow(y, 2.0f))); // squareroot of (x^2 + y^2) = length

	return length;
}

Vector2 Vector2::normalize()
{
	Vector2 normalizedVector(0.0f, 0.0f);
	float length = this->length();

	if (length != 0)
	{
		normalizedVector.x = x / length;
		normalizedVector.y = y / length;
	}

	return normalizedVector;
}