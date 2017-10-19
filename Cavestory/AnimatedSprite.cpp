#include "AnimatedSprite.h"
#include "Game.h"


AnimatedSprite::AnimatedSprite(Graphics &graphics,
	const std::string &filePath,
	int sourceX, int sourceY,
	int width, int height,
	int fps, int numOfFrames) 
	: 
	Sprite(graphics, filePath, sourceX, sourceY, width, height),
	frameTime_(1000 / fps),
	numOfFrames_(numOfFrames),
	currentFrame_(0),
	elapsedTime_(0)
{

}

void AnimatedSprite::update(int elapsedTimeMs)
{
	elapsedTime_ += elapsedTimeMs;
	if (elapsedTime_ > frameTime_)
	{
		++currentFrame_;
		elapsedTime_ = 0;
		if (currentFrame_ < numOfFrames_)
		{
			sourceRect_.x += Game::kTileSize;
		}
		else
		{
			sourceRect_.x -= Game::kTileSize * (numOfFrames_ - 1);
			currentFrame_ = 0;
		}
	}
}