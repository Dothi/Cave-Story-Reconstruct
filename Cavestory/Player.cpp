#include "Player.h"

namespace
{
	// Walk Motion
	const float kWalkingAcceleration = 0.00083007812; // (pixels / ms) / ms
	const float kMaxSpeedX = 0.15859375; // pixels / ms
	const float kFriction = 0.00049804687; // (pixels / ms) / ms

	// Fall Motion
	const float kGravity = 0.00078125; // (pixels / ms) / ms
	const float kMaxSpeedY = 0.2998046875; // pixels / ms

	// Jump Motion
	const float kJumpSpeed = 0.25f;
	const float kJumpGravity = 0.0003125f;
	const float kAirAcceleration = 0.0003125f;

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
	onGround_(false),
	jumpActive_(false),
	interacting_(false),
	polarStar_(graphics)
{
	initializeSprites(graphics);
}

void Player::update(int elapsedTimeMs, const Map &map)
{
	sprites_[getSpriteState()]->update(elapsedTimeMs);

	updateX(elapsedTimeMs, map);
	updateY(elapsedTimeMs, map);
}

void Player::updateX(int elapsedTimeMs, const Map &map)
{
	// Update velocity
	float accelerationX = 0.0f;

	if (accelerationX_ < 0) accelerationX = onGround() ? -kWalkingAcceleration : -kAirAcceleration;
	else if (accelerationX_ > 0) accelerationX = onGround() ? kWalkingAcceleration : kAirAcceleration;

	velocity_.x += accelerationX * elapsedTimeMs;

	if (accelerationX_ < 0)
	{
		velocity_.x = std::fmax(velocity_.x, -kMaxSpeedX);
	}
	else if (accelerationX_ > 0)
	{
		velocity_.x = std::fmin(velocity_.x, kMaxSpeedX);
	}
	else if (onGround())
	{
		velocity_.x = velocity_.x > 0.0f ?
			std::fmax(0.0f, velocity_.x - kFriction * elapsedTimeMs) :
			std::fmin(0.0f, velocity_.x + kFriction * elapsedTimeMs);
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
	const float gravity = jumpActive_ && velocity_.y < 0.0f ?
		kJumpGravity : kGravity;


	velocity_.y = std::fmin(velocity_.y + gravity * elapsedTimeMs, kMaxSpeedY);


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
	polarStar_.draw(graphics, horizontalFacing_, verticalFacing_, position_);
	sprites_[getSpriteState()]->draw(graphics, position_.x, position_.y);
}

void Player::startMovingLeft()
{
	accelerationX_ = -1;
	horizontalFacing_ = LEFT;
	interacting_ = false;
}

void Player::startMovingRight()
{
	accelerationX_ = 1;
	horizontalFacing_ = RIGHT;
	interacting_ = false;
}

void Player::stopMoving()
{
	accelerationX_ = 0;
}

void Player::startJump()
{
	interacting_ = false;
	jumpActive_ = true;
	if (onGround())
	{
		velocity_.y = -kJumpSpeed;
	}
}

void Player::stopJump()
{
	jumpActive_ = false;
}

void Player::lookUp()
{
	verticalFacing_ = UP;
	interacting_ = false;
}

void Player::lookDown()
{
	if (verticalFacing_ == DOWN) return;
	verticalFacing_ = DOWN;
	interacting_ = onGround() && velocity_.x == 0.0f;
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
	case INTERACTING:
		source.x = kBackFrame * Game::kTileSize;
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
		if (spriteState.verticalFacing_ == DOWN && (spriteState.motionType_ == JUMPING || spriteState.motionType_ == FALLING))
		{
			source.x = kDownFrame * Game::kTileSize;
		}
		sprites_[spriteState] = new Sprite(graphics, kSpriteFilePath, (int)round(source.x), (int)round(source.y), Game::kTileSize, Game::kTileSize);
	}
}

Player::SpriteState Player::getSpriteState()
{
	MotionType motion;

	if (interacting_)
	{
		motion = INTERACTING;
	}
	else if (onGround())
	{
		motion = accelerationX_ == 0 ? STANDING : WALKING;
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