#include "Backdrop.h"
#include "Game.h"

#include <SDL.h>

namespace
{
	const int kBackgroundSize = 128; // pixels
}

FixedBackdrop::FixedBackdrop(const std::string &path, Graphics &graphics)
{
	surfaceID_ = graphics.loadImage(path);
}

void FixedBackdrop::draw(Graphics &graphics) const
{
	for (int x = 0; x < Game::kScreenWidth; x += kBackgroundSize)
	{
		for (int y = 0; y < Game::kScreenHeight; y += kBackgroundSize)
		{
			SDL_Rect destinationRectangle;
			destinationRectangle.x = x;
			destinationRectangle.y = y;
			graphics.blitSurface(surfaceID_, NULL, &destinationRectangle);
		}
	}
}
