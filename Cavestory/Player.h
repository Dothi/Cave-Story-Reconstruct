#ifndef PLAYER_H_
#define PLAYER_H_

#include <cmath>
#include <map>
#include <string>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Vector2.h"

struct Graphics;

struct Player
{
	Player(Graphics &graphics, int x, int y);

	void update(int elapsedTimeMs);
	void draw(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void startJump();
	void stopJump();

	void lookUp();
	void lookDown();
	void lookHorizontal();

private:

	enum MotionType
	{
		FIRST_MOTION_TYPE,
		STANDING = FIRST_MOTION_TYPE,
		WALKING,
		JUMPING,
		FALLING,
		LAST_MOTION_TYPE
	};
	enum HorizontalFacing
	{
		FIRST_HORIZONTAL_FACING,
		LEFT = FIRST_HORIZONTAL_FACING,
		RIGHT,
		LAST_HORIZONTAL_FACING
	};
	enum VerticalFacing
	{
		FIRST_VERTICAL_FACING,
		UP = FIRST_VERTICAL_FACING,
		DOWN,
		HORIZONTAL,
		LAST_VERTICAL_FACING
	};

	// SpriteState of the player
	struct SpriteState
	{
		SpriteState(MotionType motionType = STANDING, 
			HorizontalFacing horizontalFacing = LEFT, VerticalFacing verticalFacing = HORIZONTAL) :
			motionType_(motionType), 
			horizontalFacing_(horizontalFacing), verticalFacing_(verticalFacing) {}

		MotionType motionType_;
		HorizontalFacing horizontalFacing_;
		VerticalFacing verticalFacing_;
	};

	struct Jump
	{
		Jump() : timeRemainingMs_(0), active_(false) {}

		void update(int elapsedTimeMs);
		void reset();
		void reactivate() { active_ = timeRemainingMs_ > 0; }
		void deactivate() { active_ = false; }
		bool active() const { return active_; }

	private:
		int timeRemainingMs_;
		bool active_;
	};

	friend bool operator<(const SpriteState &a, const SpriteState &b);

	void initializeSprites(Graphics &graphics);
	void initializeSprite(Graphics &graphics, const SpriteState &spriteState);
	SpriteState getSpriteState();

	bool onGround() const { return onGround_; }

	Vector2 position_;
	Vector2 velocity_;
	
	float accelerationX_;
	HorizontalFacing horizontalFacing_;
	VerticalFacing verticalFacing_;
	bool onGround_;
	Jump jump_;
	std::map<SpriteState, Sprite*> sprites_;
};

#endif