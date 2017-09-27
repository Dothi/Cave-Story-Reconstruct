#ifndef ANIMATED_SPRITE_H_
#define ANIMATED_SPRITE_H_

#include "Sprite.h"
#include "Game.h"

struct AnimatedSprite : public Sprite
{
	AnimatedSprite(Graphics &graphics,
		const std::string &filePath,
		int sourceX, int sourceY,
		int width, int height,
		int fps, int numOfFrames);

	void update(int elapsedTimeMs);

private:
	const int frameTime_;
	const int numOfFrames_;
	int currentFrame_;
	int elapsedTime_; // Elapsed time since the last frame change.
};

#endif // ANIMATED_SPRITE_H_