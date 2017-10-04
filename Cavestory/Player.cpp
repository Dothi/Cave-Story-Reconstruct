#include "Player.h"

namespace
{
	// Walk Motion
	const float kSlowdownFactor = 0.8f;
	const float kWalkingAcceleration = 0.0012f;
	const float kMaxSpeedX = 0.325f;

	// Jump Motion
	const float kJumpSpeed = 0.325f;
	const int kJumpTime = 275; //ms

	// Fall Motion
	const float kGravity = 0.0012f;
	const float kMaxSpeedY = 0.325f;

	// Sprites
	const std::string kSpriteFilePath("../content/MyChar.bmp");

	// Sprite Frames
	const int kCharacterFrame = 0; // in rows, not columns
	const int kWalkFrame = 0;
	const int kStandFrame = 0;
	const int kJumpFrame = 1;
	const int kFallFrame = 2;
	const int kUpFrameOffset = 3;
	const int kDownFrame = 6;
	const int kBackFrame = 7;

	// Walk Animation
	const int kWalkFps = 12;
	const int kNumWalkFrames = 3;

	// Collision Rectangle
	const Rectangle kCollisionX = Rectangle(6, 10, 20, 12);
	const Rectangle kCollisionY = Rectangle(10, 2, 12, 30);
}

bool operator<(const Player::SpriteState &a, const Player::SpriteState &b)
{
	return std::tie(a.motionType_, a.horizontalFacing_, a.verticalFacing_) < std::tie(b.motionType_, b.horizontalFacing_, b.verticalFacing_);
}

Player::Player(Graphics &graphics, int x, int y) :
	position_(x, y),
	velocity_(0.0f, 0.0f),
	accelerationX_(0),
	horizontalFacing_(LEFT),
	verticalFacing_(HORIZONTAL),
	onGround_(false)
{
	initializeSprites(graphics);
}

void Player::update(int elapsedTimeMs, const Map &map)
{
	sprites_[getSpriteState()]->update(elapsedTimeMs);
	jump_.update(elapsedTimeMs);

	updateX(elapsedTimeMs, map);
	updateY(elapsedTimeMs, map);

	/*if (position_.y > 320)
	{
		position_.y = 320;
		velocity_.y = 0.0f;
	}
	onGround_ = position_.y == 320;*/

}

void Player::updateX(int elapsedTimeMs, const Map &map)
{
	// Update velocity
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

	// Calculate delta
	const int delta = (int)round(velocity_.x * elapsedTimeMs);
	// Going right
	if (delta > 0)
	{
		// Check collision in the dir of delta
		CollisionInfo colInfo = colInfo.getWallCollisionInfo(map, rightCollision(delta));
		// React to collision
		if (colInfo.collided)
		{
			position_.x = colInfo.col * Game::kTileSize - kCollisionX.right();
			velocity_.x = 0.0f;
		}
		else
		{
			position_.x += delta;
		}

		// Check collision in other direction
		colInfo = colInfo.getWallCollisionInfo(map, leftCollision(0));

		if (colInfo.collided)
		{
			position_.x = colInfo.col * Game::kTileSize + kCollisionX.right();
		}
	}
	else
	{
		// Check collision in the dir of delta
		CollisionInfo colInfo = colInfo.getWallCollisionInfo(map, leftCollision(delta));
		// React to collision
		if (colInfo.collided)
		{
			//position_.x = colInfo.col * Game::kTileSize + kCollisionX.right();
			position_.x = colInfo.col * Game::kTileSize + Game::kTileSize - kCollisionX.left();
			velocity_.x = 0.0f;
		}
		else
		{
			position_.x += delta;
		}

		// Check collision in other direction
		colInfo = colInfo.getWallCollisionInfo(map, rightCollision(0));

		if (colInfo.collided)
		{
			position_.x = colInfo.col * Game::kTileSize - kCollisionX.right();
		}
	}
}

void Player::updateY(int elapsedTimeMs, const Map &map)
{
	// Update velocity
	if (!jump_.active())
	{
		velocity_.y = std::fmin(velocity_.y + kGravity * elapsedTimeMs, kMaxSpeedY);
	}

	// Calculate delta
	const int delta = round(velocity_.y * elapsedTimeMs);

	//going down 
	if (delta > 0)
	{
		// Check collision in the dir of delta
		CollisionInfo colInfo = colInfo.getWallCollisionInfo(map, bottomCollision(delta));
		// React to collision
		if (colInfo.collided)
		{
			position_.y = colInfo.row * Game::kTileSize - kCollisionY.bottom();
			velocity_.y = 0.0f;
			onGround_ = true;
		}
		else
		{
			position_.y += delta;
			onGround_ = false;
		}

		// Check collision in other direction
		colInfo = colInfo.getWallCollisionInfo(map, topCollision(0));

		if (colInfo.collided)
		{
			position_.y = colInfo.row * Game::kTileSize + kCollisionY.getHeight();
		}
	}
	else
	{
		CollisionInfo colInfo = colInfo.getWallCollisionInfo(map, topCollision(delta));
		// React to collision
		if (colInfo.collided)
		{
			position_.y = colInfo.row * Game::kTileSize + Game::kTileSize - kCollisionY.top();
			velocity_.y = 0.0f;
		}
		else
		{
			position_.y += delta;
			onGround_ = false;
		}

		// Check collision in other direction
		colInfo = colInfo.getWallCollisionInfo(map, bottomCollision(0));
		if (colInfo.collided)
		{
			position_.y = colInfo.row * Game::kTileSize - kCollisionY.bottom();
			onGround_ = true;
		}
	}
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

void Player::lookUp()
{
	verticalFacing_ = UP;
}

void Player::lookDown()
{
	verticalFacing_ = DOWN;
}

void Player::lookHorizontal()
{
	verticalFacing_ = HORIZONTAL;
}

void Player::initializeSprites(Graphics &graphics)
{
	for (int motionType = FIRST_MOTION_TYPE;
		motionType < LAST_MOTION_TYPE;
		++motionType)
	{
		for (int horizontalFacing = FIRST_HORIZONTAL_FACING;
			horizontalFacing < LAST_HORIZONTAL_FACING;
			++horizontalFacing)
		{
			for (int verticalFacing = FIRST_VERTICAL_FACING;
				verticalFacing < LAST_VERTICAL_FACING;
				++verticalFacing)
			{
				initializeSprite(graphics, SpriteState(
					(MotionType)motionType,
					(HorizontalFacing)horizontalFacing,
					(VerticalFacing)verticalFacing));
			}
		}
	}
}

void Player::initializeSprite(Graphics &graphics, const SpriteState &spriteState)
{
	Vector2 source;

	switch (spriteState.motionType_)
	{
	case WALKING:
		source.x = kWalkFrame * Game::kTileSize;
		break;
	case STANDING:
		source.x = kStandFrame * Game::kTileSize;
		break;
	case JUMPING:
		source.x = kJumpFrame * Game::kTileSize;
		break;
	case FALLING:
		source.x = kFallFrame * Game::kTileSize;
		break;
	case LAST_MOTION_TYPE:
		break;
	}

	source.x = spriteState.verticalFacing_ == UP ?
		source.x + kUpFrameOffset * Game::kTileSize :
		source.x;

	source.y = spriteState.horizontalFacing_ == LEFT ? kCharacterFrame * Game::kTileSize
		: (1 + kCharacterFrame) * Game::kTileSize;

	if (spriteState.motionType_ == WALKING)
	{
		sprites_[spriteState] = new AnimatedSprite(graphics, kSpriteFilePath, (int)round(source.x), (int)round(source.y), Game::kTileSize, Game::kTileSize, kWalkFps, kNumWalkFrames);
	}
	else
	{
		if (spriteState.verticalFacing_ == DOWN)
		{
			source.x = spriteState.motionType_ == STANDING ?
				kBackFrame * Game::kTileSize :
				kDownFrame * Game::kTileSize;
		}
		sprites_[spriteState] = new Sprite(graphics, kSpriteFilePath, (int)round(source.x), (int)round(source.y), Game::kTileSize, Game::kTileSize);
	}
}

Player::SpriteState Player::getSpriteState()
{
	MotionType motion;

	if (onGround())
	{
		motion = accelerationX_ == 0.0f ? STANDING : WALKING;
	}
	else
	{
		motion = velocity_.y < 0.0f ? JUMPING : FALLING;
	}

	return SpriteState(
		motion,
		horizontalFacing_,
		verticalFacing_);
}

Rectangle Player::leftCollision(int delta) const
{
	return Rectangle(
		position_.x + kCollisionX.left() + delta,
		position_.y + kCollisionX.top(),
		kCollisionX.getWidth() / 2 - delta,
		kCollisionX.getHeight());
}

Rectangle Player::rightCollision(int delta) const
{
	return Rectangle(
		position_.x + kCollisionX.left() + kCollisionX.getWidth() / 2,
		position_.y + kCollisionX.top(),
		kCollisionX.getWidth() / 2 + delta,
		kCollisionX.getHeight());
}

Rectangle Player::topCollision(int delta) const
{
	return Rectangle(
		position_.x + kCollisionY.left(),
		position_.y + kCollisionY.top() + delta,
		kCollisionY.getWidth(),
		kCollisionY.getHeight() / 2 - delta);
}

Rectangle Player::bottomCollision(int delta) const
{
	return Rectangle(
		position_.x + kCollisionY.left(),
		position_.y + kCollisionY.top() + kCollisionY.getHeight() / 2,
		kCollisionY.getWidth(),
		kCollisionY.getHeight() / 2 + delta);
}

Player::SpriteState::SpriteState(
	MotionType motionType = STANDING,
	HorizontalFacing horizontalFacing = LEFT,
	VerticalFacing verticalFacing = HORIZONTAL) :
	motionType_(motionType),
	horizontalFacing_(horizontalFacing),
	verticalFacing_(verticalFacing)
{

}

Player::Jump::Jump() : timeRemainingMs_(0), active_(false)
{

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

void Player::Jump::reactivate()
{
	active_ = timeRemainingMs_ > 0;
}

void Player::Jump::deactivate()
{
	active_ = false;
}

Player::CollisionInfo Player::CollisionInfo::getWallCollisionInfo(const Map &map, const Rectangle &rectangle)
{
	std::vector<Map::CollisionTile> tiles = map.getCollidingTiles(rectangle);

	CollisionInfo colInfo = { false, 0, 0 };

	for (size_t i = 0; i < tiles.size(); ++i)
	{
		if (tiles[i].tileType_ == Map::WALL_TILE)
		{
			colInfo = { true, tiles[i].row_, tiles[i].col_ };
			break;
		}
	}
	return colInfo;
}