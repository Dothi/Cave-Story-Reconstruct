#include "Sprite.h"

Sprite::Sprite(
	Graphics &graphics,
	const std::string &filePath,
	int sourceX, int sourceY,
	int width, int height)
{
	const bool blackIsTransparent = true;
	spriteSheet_ = graphics.loadImage(filePath, blackIsTransparent);

	sourceRect_.x = sourceX;
	sourceRect_.y = sourceY;
	sourceRect_.w = width;
	sourceRect_.h = height;
}


void Sprite::draw(Graphics &graphics, int x, int y)
{
	SDL_Rect destinationRect;
	destinationRect.x = x;
	destinationRect.y = y;
	graphics.blitSurface(spriteSheet_, &sourceRect_, &destinationRect);
}