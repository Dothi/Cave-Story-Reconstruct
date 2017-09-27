#include "Graphics.h"

namespace
{
	const int kScreenWidth = 640;
	const int kScreenHeight = 480;
}

Graphics::SurfaceID Graphics::loadImage(const std::string filePath)
{
	// if we have not loaded spritesheet
	if (spriteSheets_.count(filePath) == 0)
	{
		// load it now
		spriteSheets_[filePath] = SDL_LoadBMP(filePath.c_str());
	}
	return spriteSheets_[filePath];
}

Graphics::Graphics()
{
	window_ = SDL_CreateWindow("Cavestory",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		kScreenWidth, kScreenHeight,
		NULL);
	screen_ = SDL_GetWindowSurface(window_);

	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

Graphics::~Graphics()
{
	for (SpriteMap::iterator it = spriteSheets_.begin(); it != spriteSheets_.end(); ++it)
	{
		SDL_FreeSurface(it->second);
	}
	SDL_DestroyWindow(window_);
}

void Graphics::blitSurface(
	SurfaceID source,
	SDL_Rect* sourceRect,
	SDL_Rect* destinationRect)
{
	SDL_BlitSurface(source, sourceRect, screen_, destinationRect);
}

void Graphics::clear()
{
	SDL_FillRect(screen_, nullptr, 0);
}

void Graphics::updateWindowSurface()
{
	SDL_UpdateWindowSurface(window_);
}

