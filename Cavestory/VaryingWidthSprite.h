#ifndef VARYING_WIDTH_SPRITE_H_
#define VARYING_WIDTH_SPRITE_H_

#include "Sprite.h"

struct VaryingWidthSprite : Sprite
{
	VaryingWidthSprite(Graphics &graphics,
		const std::string &filePath,
		int sourceX, int sourceY,
		int width, int height) :
		Sprite(graphics, filePath, sourceX, sourceY, width, height) {}

	void setWidth(int width) { sourceRect_.w = width; }
};

#endif // VARYING_WIDTH_SPRITE_H_
