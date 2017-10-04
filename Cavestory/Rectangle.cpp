#include "Rectangle.h"


Rectangle::Rectangle(int x, int y, int width, int height) :
	x_(x), y_(y), width_(width), height_(height)
{

}

int Rectangle::left() const
{
	return x_;
}

int Rectangle::right() const
{
	return x_ + width_;
}

int Rectangle::top() const
{
	return y_;
}

int Rectangle::bottom() const
{
	return y_ + height_;
}