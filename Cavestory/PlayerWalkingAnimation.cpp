#include "Player.h"

// Walk Animation
const int kWalkFps = 15;
const int kNumWalkFrames = 3;

Player::WalkingAnimation::WalkingAnimation() :
	frameTimer_(1000/kWalkFps),
	currentIndex_(0),
	forward_(true)

{

}

void Player::WalkingAnimation::update()
{
	if (frameTimer_.expired())
	{
		frameTimer_.reset();

		if (forward_)
		{
			++currentIndex_;
			forward_ = currentIndex_ != kNumWalkFrames - 1;
		}
		else
		{
			--currentIndex_;
			forward_ = currentIndex_ == 0;
		}
	}
}

void Player::WalkingAnimation::reset()
{
	forward_ = true;
	currentIndex_ = 0;
	frameTimer_.reset();
}

Player::StrideType Player::WalkingAnimation::stride() const
{
	switch (currentIndex_)
	{
	case 0:
		return STRIDE_LEFT;
	case 1:
		return STRIDE_MIDDLE;
	case 2:
		return STRIDE_RIGHT;
	default:
		return STRIDE_MIDDLE;

	}
}