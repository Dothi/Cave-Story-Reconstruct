#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include "Vector2.h"
#include <vector>

struct Sprite;
struct Graphics;

struct NumberSprite
{
	NumberSprite(Graphics &graphics, int number, int numDigits = 0);

	void draw(Graphics &graphics, int x, int y);
private:
	int padding_;
	std::vector<Sprite*> reversedDigits_;
};

#endif
