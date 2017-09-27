#ifndef PLAYER_H_
#define PLAYER_H_

#include <cmath>
#include <map>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Graphics.h"

struct Graphics;

struct Player
{
	Player(Graphics &graphics, int x, int y);

	void update(int elapsedTimeMs);
	void draw(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();
private:
	enum MotionType
	{
		STANDING,
		WALKING
	};
	enum HorizontalFacing
	{
		LEFT,
		RIGHT
	};
	// SpriteState of the player
	struct SpriteState
	{
		SpriteState(MotionType motionType = STANDING, HorizontalFacing horizontalFacing = LEFT) :
			motionType_(motionType), horizontalFacing_(horizontalFacing) {}

		MotionType motionType_;
		HorizontalFacing horizontalFacing_;
	};

	friend bool operator<(const SpriteState &a, const SpriteState &b);

	void initializeSprites(Graphics &graphics);
	SpriteState getSpriteState();

	int x_, y_;
	float velocityX_;
	float accelerationX_;
	HorizontalFacing horizontalFacing_;
	std::map<SpriteState, Sprite*> sprites_;
};

#endif