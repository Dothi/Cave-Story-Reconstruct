#include "Player.h"

namespace
{
	// animation
	const int kCharacterAnimationFps = 12;
	const int kCharacterAnimationFrames = 3;

	// movement
	const float kSlowdownFactor = 0.8f;
	const float kWalkingAcceleration = 0.0012f;
	const float kMaxSpeedX = 0.325f;
}

bool operator<(const Player::SpriteState &a, const Player::SpriteState &b)
{
	return std::tie(a.motionType_, a.horizontalFacing_) < std::tie(b.motionType_, b.horizontalFacing_);
}

Player::Player(Graphics &graphics, int x, int y) : 
x_(x), y_(y),
velocityX_(0), accelerationX_(0),
horizontalFacing_(LEFT)
{
	initializeSprites(graphics);
}

void Player::update(int elapsedTimeMs)
{
	sprites_[getSpriteState()]->update(elapsedTimeMs);

	x_ += round(velocityX_ * elapsedTimeMs);
	velocityX_ += accelerationX_ * elapsedTimeMs;

	if (accelerationX_ < 0.0f)
	{
		velocityX_ = std::fmax(velocityX_, -kMaxSpeedX);
	}
	else if (accelerationX_ > 0.0f)
	{
		velocityX_ = std::fmin(velocityX_, kMaxSpeedX);
	}
	else
	{
		velocityX_ *= kSlowdownFactor;
	}
}

void Player::draw(Graphics &graphics)
{
	sprites_[getSpriteState()]->draw(graphics, x_, y_);
}

void Player::startMovingLeft()
{
	accelerationX_ = -kWalkingAcceleration;
	horizontalFacing_ = LEFT;
}

void Player::startMovingRight()
{
	accelerationX_ = kWalkingAcceleration;
	horizontalFacing_ = RIGHT;
}

void Player::stopMoving()
{
	accelerationX_ = 0.0f;
}

void Player::initializeSprites(Graphics &graphics)
{
	sprites_[SpriteState(STANDING, LEFT)] = new Sprite(graphics, "../content/MyChar.bmp", 0, 0, Game::kTileSize, Game::kTileSize);
	sprites_[SpriteState(WALKING, LEFT)] = new AnimatedSprite(graphics, "../content/MyChar.bmp", 0, 0, Game::kTileSize, Game::kTileSize, kCharacterAnimationFps, kCharacterAnimationFrames);
	sprites_[SpriteState(STANDING, RIGHT)] = new Sprite(graphics, "../content/MyChar.bmp", 0, Game::kTileSize, Game::kTileSize, Game::kTileSize);
	sprites_[SpriteState(WALKING, RIGHT)] = new AnimatedSprite(graphics, "../content/MyChar.bmp", 0, Game::kTileSize, Game::kTileSize, Game::kTileSize, kCharacterAnimationFps, kCharacterAnimationFrames);
}

Player::SpriteState Player::getSpriteState()
{
	return SpriteState(accelerationX_ == 0.0f ? STANDING : WALKING,
		horizontalFacing_);
}