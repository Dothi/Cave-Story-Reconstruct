#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include <SDL.h>
#include <string>
#include <map>


struct Graphics
{
	typedef SDL_Surface *SurfaceID;

	Graphics();
	~Graphics();

	SurfaceID loadImage(const std::string filePath, bool blackIsTransparent = true);

	void blitSurface(
		SurfaceID source,
		SDL_Rect *sourceRect,
		SDL_Rect *destinationRect);

	void clear();

	void updateWindowSurface();
private:
	typedef std::map<std::string, SDL_Surface*> SpriteMap;

	SpriteMap spriteSheets_;
	SDL_Window *window_;
	SDL_Surface *screen_;
};

#endif