#include "NumberSprite.h"
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"

#include <assert.h>
#include <string>

namespace
{
	const std::string kSpritePath("../content/TextBox.bmp");

	const int kSourceY = 7 * Game::kHalfTile;
	const int kSourceWidth = Game::kHalfTile;
	const int kSourceHeight = Game::kHalfTile;
}

NumberSprite::NumberSprite(Graphics &graphics, int number, int numDigits) :
	padding_(0)
{
	assert(number >= 0);

	int digitCount = 0;
	do
	{
		const int digit = number % 10;
		reversedDigits_.push_back(new Sprite(graphics, kSpritePath,
			digit * Game::kHalfTile, kSourceY,
			kSourceWidth, kSourceHeight));

		number /= 10;
		++digitCount;
	} 
	while (number != 0);

	
	assert(numDigits == 0 || numDigits >= digitCount);
	padding_ = numDigits == 0 ? 0 : Game::kHalfTile * (numDigits - digitCount);
}

void NumberSprite::draw(Graphics &graphics, int x, int y)
{
	for (size_t i = 0; i < reversedDigits_.size(); ++i)
	{
		const int offset = Game::kHalfTile * (reversedDigits_.size() - 1 - i);
		reversedDigits_[i]->draw(graphics, x + offset + padding_, y);

	}
}