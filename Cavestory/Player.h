#ifndef PLAYER_H_
#define PLAYER_H_

#include <cmath>
#include <map>
#include <string>
#include "Sprite.h"
#include "SpriteState.h"
#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Vector2.h"
#include "Map.h"
#include "Rectangle.h"
#include "PolarStar.h"
#include "Timer.h"

struct Graphics;
struct Map;

struct Player
{
	Player(Graphics &graphics, int x, int y);

	void update(int elapsedTimeMs, const Map &map);

	void updateX(int elapsedTimeMs, const Map &map);
	void updateY(int elapsedTimeMs, const Map &map);

	void draw(Graphics &graphics);

	void startMovingLeft();
	void startMovingRight();
	void stopMoving();

	void startJump();
	void stopJump();

	void startFire();
	void stopFire();

	void lookUp();
	void lookDown();
	void lookHorizontal();

	void takeDamage();

	Rectangle damageRectangle() const;

	Vector2 getPosition() const { return position_; }

private:

	enum MotionType
	{
		FIRST_MOTION_TYPE,
		STANDING = FIRST_MOTION_TYPE,
		INTERACTING,
		WALKING,
		JUMPING,
		FALLING,
		LAST_MOTION_TYPE
	};
	enum StrideType
	{
		FIRST_STRIDE_TYPE,
		STRIDE_MIDDLE = FIRST_STRIDE_TYPE,
		STRIDE_LEFT,
		STRIDE_RIGHT,
		LAST_STRIDE_TYPE
	};
	

	// SpriteState of the player
	struct SpriteState
	{
		SpriteState(
			MotionType motionType,
			HorizontalFacing horizontalFacing, 
			VerticalFacing verticalFacing,
			StrideType strideType);

		MotionType motionType_;
		HorizontalFacing horizontalFacing_;
		VerticalFacing verticalFacing_;
		StrideType strideType_;
	};

	struct WalkingAnimation
	{
		WalkingAnimation();
		void update();
		void reset();

		StrideType stride() const;
	private:
		Timer frameTimer_;
		int currentIndex_;
		bool forward_;
	};

	struct CollisionInfo
	{
		bool collided;
		int row, col;

		CollisionInfo getWallCollisionInfo(const Map &map, const Rectangle &rectangle);
	};

	friend bool operator<(const SpriteState &a, const SpriteState &b);

	void initializeSprites(Graphics &graphics);
	void initializeSprite(Graphics &graphics, const SpriteState &spriteState);
	SpriteState getSpriteState();

	Rectangle leftCollision(int delta) const;
	Rectangle rightCollision(int delta) const;
	Rectangle topCollision(int delta) const;
	Rectangle bottomCollision(int delta) const;


	MotionType motionType() const;
	bool onGround() const { return onGround_; }
	bool gunUp() const { return motionType() == WALKING && walkingAnimation_.stride() != STRIDE_MIDDLE; }
	VerticalFacing verticalFacing() const { return onGround() && intendedVerticalFacing_ == DOWN ?
		HORIZONTAL : intendedVerticalFacing_; }

	Vector2 position_;
	Vector2 velocity_;
	
	int accelerationX_;
	HorizontalFacing horizontalFacing_;
	VerticalFacing intendedVerticalFacing_;
	bool onGround_;
	bool jumpActive_;
	bool interacting_;


	bool invincible_;
	int invincibleTime_;
	WalkingAnimation walkingAnimation_;

	PolarStar polarStar_;

	std::map<SpriteState, Sprite*> sprites_;
};

#endif