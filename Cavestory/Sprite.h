#ifndef SPRITE_H_
#define SPRITE_H_

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include "Graphics.h"

struct Sprite
{
	Sprite(Graphics &graphics, 
		const std::string &filePath,
		int sourceX, int sourceY,
		int width, int height);

	virtual void update(int elapsedTimeMs) {};
	void draw(Graphics &graphics, int x, int y);

protected:
	SDL_Rect sourceRect_;

private:
	SDL_Surface *spriteSheet_;
	
};

#endif