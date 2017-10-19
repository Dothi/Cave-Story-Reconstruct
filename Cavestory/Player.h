#ifndef PLAYER_H_
#define PLAYER_H_

#include <cmath>
#include <map>
#include <string>
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Graphics.h"
#include "Vector2.h"
#include "Map.h"
#include "Rectangle.h"

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

	void lookUp();
	void lookDown();
	void lookHorizontal();

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
		SpriteState(
			MotionType motionType,
			HorizontalFacing horizontalFacing, 
			VerticalFacing verticalFacing);

		MotionType motionType_;
		HorizontalFacing horizontalFacing_;
		VerticalFacing verticalFacing_;
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

	bool onGround() const { return onGround_; }

	Vector2 position_;
	Vector2 velocity_;
	
	int accelerationX_;
	HorizontalFacing horizontalFacing_;
	VerticalFacing verticalFacing_;
	bool onGround_;
	bool jumpActive_;
	bool interacting_;

	std::map<SpriteState, Sprite*> sprites_;
};

#endif