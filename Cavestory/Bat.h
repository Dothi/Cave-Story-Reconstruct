#ifndef BAT_H_
#define BAT_H_

#include "Vector2.h"
#include "Rectangle.h"
#include "Game.h"
#include <map>


struct Graphics;
struct Sprite;


struct Bat
{
	Bat(Graphics &graphics, Vector2 position);
	void update(int elapsedTime, Vector2 playerPos);
	void draw(Graphics &graphics) const;

	Rectangle damageRectangle() const 
	{
		return Rectangle(
			position_.x + Game::kTileSize / 2.0f, 
			position_.y + Game::kTileSize / 2.0f,
			0, 0);
	}
private:
	enum Facing
	{
		FIRST_FACING,
		LEFT = FIRST_FACING,
		RIGHT,
		LAST_FACING
	};
	struct SpriteState
	{ 
		SpriteState(Facing facing) : facing_(facing) {}

		Facing facing_;
	};
	friend bool operator<(const SpriteState &a, const SpriteState &b)
	{
		return a.facing_ < b.facing_;
	}

	void initializeSprites(Graphics &graphics);
	void initializeSprite(Graphics &graphics, const SpriteState &spriteState);
	SpriteState getSpriteState() const;

	const int centerY_;
	Vector2 position_;
	float flightAngle_;
	Facing facing_;
	std::map<SpriteState, Sprite*> sprites_;
	
	float degreesToRadians(float degrees) const;
};

#endif
