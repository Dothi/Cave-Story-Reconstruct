#include "Bat.h"
#include "AnimatedSprite.h"
#include "Graphics.h"

namespace
{
	const int kFlyFps = 13;
	const int kNumFlyFrames = 3;

	const float kAngularVelocity = 120.0f / 1000.0f; //ms
}

Bat::Bat(Graphics &graphics, Vector2 position) :
	centerY_(position.y),
	position_(position),
	flightAngle_(0.0f),
	facing_(RIGHT)
{
	initializeSprites(graphics);
	
}

void Bat::update(int elapsedTime, Vector2 playerPos)
{
	flightAngle_ += kAngularVelocity * elapsedTime;

	const float center = position_.x + Game::kTileSize / 2.0f;
	const float playerCenter = playerPos.x + Game::kTileSize / 2.0f;

	facing_ = center > playerCenter ?
		LEFT : RIGHT;

	position_.y = centerY_ + Game::kTileSize * 5 / 2.0f * std::sin(degreesToRadians(flightAngle_));

	sprites_[getSpriteState()]->update(elapsedTime);
}

void Bat::draw(Graphics &graphics) const
{
	sprites_.at(getSpriteState())->draw(graphics, position_.x, position_.y);
}

float Bat::degreesToRadians(float degrees) const
{
	return degrees * (atan(1) * 4) / 180.0f;
}

void Bat::initializeSprites(Graphics &graphics)
{
	for (int facing = FIRST_FACING; facing < LAST_FACING; ++facing)
	{
		initializeSprite(graphics, SpriteState((Facing)facing));
	}
}

void Bat::initializeSprite(Graphics &graphics, const SpriteState &spriteState)
{
	int tileY = spriteState.facing_ == RIGHT ? 3 * Game::kTileSize : 2 * Game::kTileSize;

	sprites_[spriteState] = new AnimatedSprite(graphics, "../content/NpcCemet.bmp",
		2 * Game::kTileSize, tileY,
		Game::kTileSize, Game::kTileSize,
		kFlyFps, kNumFlyFrames);
}

Bat::SpriteState Bat::getSpriteState() const
{
	return SpriteState(facing_);
}