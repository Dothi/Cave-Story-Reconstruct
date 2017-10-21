#ifndef RECTANGLE_H_
#define RECTANGLE_H_

struct Rectangle
{
	Rectangle(int x, int y, int width, int height);

	int left() const;
	int right() const;
	int top() const;
	int bottom() const;

	int getWidth() const { return width_; }
	int getHeight() const { return height_; }

	bool collidesWith(const Rectangle &other) const
	{
		return right() >= other.left() &&
			left() <= other.right() &&
			top() <= other.bottom() &&
			bottom() >= other.top();
	}

private:
	const int x_, y_;
	const int width_, height_;
};

#endif
