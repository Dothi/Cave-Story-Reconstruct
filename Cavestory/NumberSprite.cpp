#include "NumberSprite.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"

#include <assert.h>
#include <string>

namespace
{
	const std::string kSpritePath("../content/TextBox.bmp");

	const int kSourceWhiteY = 7 * Game::kHalfTile;
	const int kSourceRedY = 8 * Game::kHalfTile;

	const int kOpSourceY = 6 * Game::kHalfTile;
	const int kPlusSourceX = 4 * Game::kHalfTile;
	const int kMinusSourceX = 5 * Game::kHalfTile;


	const int kSourceWidth = Game::kHalfTile;
	const int kSourceHeight = Game::kHalfTile;
}
// static
NumberSprite NumberSprite::HUDNumber(Graphics &graphics, int number, int numDigits)
{
	return NumberSprite(graphics, number, numDigits, WHITE, NONE);
}
//static 
NumberSprite NumberSprite::DamageNumber(Graphics &graphics, int number)
{
	return NumberSprite(graphics, number, 0, RED, MINUS);
}
// static
NumberSprite NumberSprite::ExperienceNumber(Graphics &graphics, int number)
{
	return NumberSprite(graphics, number, 0, WHITE, PLUS);
}

NumberSprite::NumberSprite(Graphics &graphics, int number, int numDigits, ColorType color, OperatorType op) :
	padding_(0)
{
	assert(number >= 0);

	int sourceY = color == RED ? kSourceRedY : kSourceWhiteY;
	int digitCount = 0;
	do
	{
		const int digit = number % 10;
		reversedSprites_.push_back(new Sprite(graphics, kSpritePath,
			digit * Game::kHalfTile, sourceY,
			kSourceWidth, kSourceHeight));

		number /= 10;
		++digitCount;
	} 
	while (number != 0);

	
	assert(numDigits == 0 || numDigits >= digitCount);
	padding_ = numDigits == 0 ? 0 : Game::kHalfTile * (numDigits - digitCount);
	switch (op)
	{
	case PLUS:
		reversedSprites_.push_back(new Sprite(graphics, kSpritePath,
			kPlusSourceX, kOpSourceY,
			kSourceWidth, kSourceHeight));
		break;
	case MINUS:
		reversedSprites_.push_back(new Sprite(graphics, kSpritePath,
			kMinusSourceX, kOpSourceY,
			kSourceWidth, kSourceHeight));
		break;
	case NONE:
		break;
	default:
		break;
	}
}

void NumberSprite::draw(Graphics &graphics, int x, int y)
{
	for (size_t i = 0; i < reversedSprites_.size(); ++i)
	{
		const int offset = Game::kHalfTile * (reversedSprites_.size() - 1 - i);
		reversedSprites_[i]->draw(graphics, x + offset + padding_, y);

	}
}

int NumberSprite::width() const
{
	return Game::kHalfTile * reversedSprites_.size();
}

int NumberSprite::height() const
{
	return Game::kHalfTile;
}