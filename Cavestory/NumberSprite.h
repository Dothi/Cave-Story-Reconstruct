#ifndef NUMBER_SPRITE_H_
#define NUMBER_SPRITE_H_

#include "Vector2.h"
#include <vector>

struct Sprite;
struct Graphics;

struct NumberSprite
{
	// Ex. '3' in white text without the operator
	static NumberSprite HUDNumber(Graphics &graphics, int number, int numDigits);
	// Ex. '-3' in red text
	static NumberSprite DamageNumber(Graphics &graphics, int number);
	// Ex. '+20' in white text
	static NumberSprite ExperienceNumber(Graphics &graphics, int number);


	void draw(Graphics &graphics, int x, int y);

	int width() const;
	int height() const;
private:
	enum ColorType
	{
		RED,
		WHITE
	};
	enum OperatorType
	{
		PLUS,
		MINUS,
		NONE
	};
	NumberSprite(Graphics &graphics, int number, int numDigits, ColorType color, OperatorType op);

	int padding_;
	std::vector<Sprite*> reversedSprites_;
};

#endif
