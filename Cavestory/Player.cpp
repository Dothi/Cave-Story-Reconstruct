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
	const float kMaxSpeedY = 0.325f;

	// jump
	const float kJumpSpeed = 0.325f;
	const int kJumpTime = 275; //ms

	const float kGravity = 0.0012f;
}

bool operator<(const Player::SpriteState &a, const Player::SpriteState &b)
{
	return std::tie(a.motionType_, a.horizontalFacing_) < std::tie(b.motionType_, b.horizontalFacing_);
}

Player::Player(Graphics &graphics, int x, int y) :
	position_(x, y),
	velocity_(0, 0),
	accelerationX_(0),
	horizontalFacing_(LEFT),
	onGround_(false)
{
	initializeSprites(graphics);
}

void Player::update(int elapsedTimeMs)
{
	sprites_[getSpriteState()]->update(elapsedTimeMs);
	jump_.update(elapsedTimeMs);

	position_.x += round(velocity_.x * elapsedTimeMs);
	velocity_.x += accelerationX_ * elapsedTimeMs;

	if (accelerationX_ < 0.0f)
	{
		velocity_.x = std::fmax(velocity_.x, -kMaxSpeedX);
	}
	else if (accelerationX_ > 0.0f)
	{
		velocity_.x = std::fmin(velocity_.x, kMaxSpeedX);
	}
	else if (onGround())
	{
		velocity_.x *= kSlowdownFactor;
	}

	position_.y += round(velocity_.y * elapsedTimeMs);

	if (!jump_.active())
	{
		velocity_.y = std::fmin(velocity_.y + kGravity * elapsedTimeMs, kMaxSpeedY);
	}

	if (position_.y > 320)
	{
		position_.y = 320;
		velocity_.y = 0.0f;
	}
	onGround_ = position_.y == 320;

}

void Player::draw(Graphics &graphics)
{
	sprites_[getSpriteState()]->draw(graphics, position_.x, position_.y);
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

void Player::startJump()
{
	if (onGround())
	{
		jump_.reset();
		velocity_.y = -kJumpSpeed;
	}
	else if (velocity_.y < 0.0f)
	{
		jump_.reactivate();
	}
}

void Player::stopJump()
{
	//deactivate jump
	jump_.deactivate();
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

void Player::Jump::update(int elapsedTimeMs)
{
	if (active_)
	{
		timeRemainingMs_ -= elapsedTimeMs;

		if (timeRemainingMs_ <= 0)
			active_ = false;
	}
}

void Player::Jump::reset()
{
	timeRemainingMs_ = kJumpTime;
	reactivate();
}