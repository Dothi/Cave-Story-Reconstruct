#include "PolarStar.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Game.h"
#include <cmath>

namespace
{
	const int kPolarStarIndex = 2;
	const int kGunWidth = 3 * Game::kTileSize / 2;
	const int kGunHeight = 2 * Game::kTileSize / 2;

	const int kHorizontalOffset = 0;
	const int kUpOffset = Game::kTileSize * 2;
	const int kDownOffset = Game::kTileSize * 4;

	const int kLeftOffset = 0;
	const int kRightOffset = Game::kTileSize;

	const std::string kSpritePath("../content/Arms.bmp");
}

PolarStar::PolarStar(Graphics &graphics)
{
	initializeSprites(graphics);
}

void PolarStar::draw(
	Graphics &graphics, 
	HorizontalFacing horizontalFacing, 
	VerticalFacing verticalFacing,
	bool gunUp,
	Vector2 position)
{
	if (horizontalFacing == LEFT)
		position.x -= Game::kTileSize / 2;
	if (verticalFacing == UP)
		position.y -= Game::kTileSize / 4;
	if (verticalFacing == DOWN)
		position.y += Game::kTileSize / 4;
	if (gunUp)
		position.y -= 2.0f;
	spriteMap_[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, (int)round(position.x), (int)round(position.y));
}

bool operator<(const PolarStar::SpriteState &a, const PolarStar::SpriteState &b)
{
	return std::tie(a.horizontalFacing_, a.verticalFacing_) < std::tie(b.horizontalFacing_, b.verticalFacing_);
}

void PolarStar::initializeSprites(Graphics &graphics)
{

	for (int horizontalFacing = FIRST_HORIZONTAL_FACING;
		horizontalFacing < LAST_HORIZONTAL_FACING;
		++horizontalFacing)
	{
		for (int verticalFacing = FIRST_VERTICAL_FACING;
			verticalFacing < LAST_VERTICAL_FACING;
			++verticalFacing)
		{
			initializeSprite(graphics, SpriteState(
				(HorizontalFacing)horizontalFacing,
				(VerticalFacing)verticalFacing));
		}
	}
}

void PolarStar::initializeSprite(Graphics &graphics, const SpriteState &spriteState)
{
	int tileY = spriteState.horizontalFacing_ == LEFT ? kLeftOffset : kRightOffset;

	switch (spriteState.verticalFacing_)
	{
	case HORIZONTAL:
		tileY += kHorizontalOffset;
		break;
	case UP:
		tileY += kUpOffset;
		break;
	case DOWN:
		tileY += kDownOffset;
		break;
	case LAST_VERTICAL_FACING:
		break;
	}

	spriteMap_[spriteState] = new Sprite(graphics, kSpritePath,
		kPolarStarIndex * kGunWidth, tileY,
		kGunWidth, kGunHeight);
}