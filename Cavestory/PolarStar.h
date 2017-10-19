#ifndef POLAR_STAR_H_
#define POLAR_STAR_H_

#include "Vector2.h"
#include "SpriteState.h"

#include <map>

struct Graphics;
struct Sprite;

struct PolarStar
{
	PolarStar(Graphics &graphics);

	void draw(Graphics &graphics, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, Vector2 position);

private:
	struct SpriteState
	{
		SpriteState(
			HorizontalFacing horizontalFacing,
			VerticalFacing verticalFacing) :
			horizontalFacing_(horizontalFacing),
			verticalFacing_(verticalFacing) {}

		HorizontalFacing horizontalFacing_;
		VerticalFacing verticalFacing_;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b);

	void initializeSprites(Graphics &graphics);
	void initializeSprite(Graphics &graphics, const SpriteState &spriteState);

	std::map<SpriteState, Sprite*> spriteMap_;
};

#endif