#include "PolarStar.h"
#include "Graphics.h"
#include "Sprite.h"
#include "Game.h"
#include <cmath>

namespace
{
	const int kPolarStarIndex = 2;
	const int kGunWidth = 3 * Game::kTileSize / 2;
	const int kGunHeight = 2 * Game::kTileSize / 2;

	const int kHorizontalOffset = 0;
	const int kUpOffset = Game::kTileSize * 2;
	const int kDownOffset = Game::kTileSize * 4;

	const int kLeftOffset = 0;
	const int kRightOffset = Game::kTileSize;

	const std::string kSpritePath("../content/Arms.bmp");

	// Nozzle Offset
	const int kNozzleHorizontalY = 23;
	const int kNozzleHorizontalLeftX = 10;
	const int kNozzleHorizontalRightX = 38;

	const int kNozzleUpY = 4;
	const int kNozzleUpLeftX = 27;
	const int kNozzleUpRightX = 21;

	const int kNozzleDownY = 28;
	const int kNozzleDownLeftX = 29;
	const int kNozzleDownRightX = 19;

	// Projectile Sprite
	const int kProjectileSourceY = 2 * Game::kTileSize;
	const int kHorizontalProjectileSourceX = 8 * Game::kTileSize;
	const int kVerticalProjectileSourceX = 9 * Game::kTileSize;

	// Projectile Velocity
	const float kProjectileSpeed = 0.6f;

	const int kProjectileMaxOffset = 14 * Game::kTileSize / 2;
	//const int kProjectileMaxOffset = 7 * Game::kTileSize / 2;
}

PolarStar::PolarStar(Graphics &graphics) : projectileA_(nullptr), projectileB_(nullptr)
{
	initializeSprites(graphics);
}

void PolarStar::updateProjectiles(int elapsedTime)
{
	if (projectileA_)
	{
		if (!projectileA_->update(elapsedTime))
		{
			projectileA_ = nullptr;
		}
	}
	if (projectileB_)
	{
		if (!projectileB_->update(elapsedTime))
		{
			projectileB_ = nullptr;
		}
	}
}

void PolarStar::draw(
	Graphics &graphics,
	HorizontalFacing horizontalFacing,
	VerticalFacing verticalFacing,
	bool gunUp,
	Vector2 playerPosition)
{
	Vector2 gunPos = Vector2(gunX(horizontalFacing, (int)round(playerPosition.x)), 
							 gunY(verticalFacing, gunUp, (int)round(playerPosition.y)));

	spriteMap_[SpriteState(horizontalFacing, verticalFacing)]->draw(graphics, gunPos.x, gunPos.y);
	
	if (projectileA_)
	projectileA_->draw(graphics);
	if (projectileB_)
		projectileB_->draw(graphics);

}

void PolarStar::startFire(
	Vector2 playerPosition,
	HorizontalFacing horizontalFacing,
	VerticalFacing verticalFacing, bool gunUp)
{
	if (projectileA_ && projectileB_) return;

	Vector2 bullet = playerPosition;
	bullet.y = gunY(verticalFacing, gunUp, bullet.y) - Game::kTileSize / 2;
	bullet.x = gunX(horizontalFacing, bullet.x) - Game::kTileSize / 2;

	switch (verticalFacing)
	{
	case HORIZONTAL:
		bullet.y += kNozzleHorizontalY;
		if (horizontalFacing == LEFT)
		{
			bullet.x += kNozzleHorizontalLeftX;
		}
		else
		{
			bullet.x += kNozzleHorizontalRightX;
		}
		break;
	case UP:
		bullet.y += kNozzleUpY;
		if (horizontalFacing == LEFT)
		{
			bullet.x += kNozzleUpLeftX;
		}
		else
		{
			bullet.x += kNozzleUpRightX;
		}
		break;
	case DOWN:
		bullet.y += kNozzleDownY;
		if (horizontalFacing == LEFT)
		{
			bullet.x += kNozzleDownLeftX;
		}
		else
		{
			bullet.x += kNozzleDownRightX;
		}
		break;
	default:
		break;
	}
	// Create our projectile using bullet vector
	if (!projectileA_)
	{
		projectileA_ = new Projectile(
			verticalFacing == HORIZONTAL ? horizontalProjectile_ : verticalProjectile_,
			horizontalFacing, verticalFacing,
			bullet);
	}
	else if (!projectileB_)
	{
		projectileB_ = new Projectile(
			verticalFacing == HORIZONTAL ? horizontalProjectile_ : verticalProjectile_,
			horizontalFacing, verticalFacing,
			bullet);
	}
	
	
}

void PolarStar::stopFire()
{

}

bool operator<(const PolarStar::SpriteState &a, const PolarStar::SpriteState &b)
{
	return std::tie(a.horizontalFacing_, a.verticalFacing_) < std::tie(b.horizontalFacing_, b.verticalFacing_);
}

int PolarStar::gunX(HorizontalFacing horizontalFacing, int playerX) const
{
	return horizontalFacing == LEFT ? playerX - Game::kTileSize / 2 : playerX;
}

int PolarStar::gunY(VerticalFacing verticalFacing, bool gunUp, int playerY) const
{
	int gunY = playerY;

	if (verticalFacing == UP)
		gunY -= Game::kTileSize / 4;
	if (verticalFacing == DOWN)
		gunY += Game::kTileSize / 4;
	if (gunUp)
		gunY -= 2.0f;

	return gunY;
}

void PolarStar::initializeSprites(Graphics &graphics)
{
	horizontalProjectile_ = new Sprite(graphics, "../content/Bullet.bmp",
		kHorizontalProjectileSourceX, kProjectileSourceY,
		Game::kTileSize, Game::kTileSize);

	verticalProjectile_ = new Sprite(graphics, "../content/Bullet.bmp",
		kVerticalProjectileSourceX, kProjectileSourceY,
		Game::kTileSize, Game::kTileSize);


	for (int horizontalFacing = FIRST_HORIZONTAL_FACING;
		horizontalFacing < LAST_HORIZONTAL_FACING;
		++horizontalFacing)
	{
		for (int verticalFacing = FIRST_VERTICAL_FACING;
			verticalFacing < LAST_VERTICAL_FACING;
			++verticalFacing)
		{
			initializeSprite(graphics, SpriteState(
				(HorizontalFacing)horizontalFacing,
				(VerticalFacing)verticalFacing));
		}
	}
}

void PolarStar::initializeSprite(Graphics &graphics, const SpriteState &spriteState)
{
	int tileY = spriteState.horizontalFacing_ == LEFT ? kLeftOffset : kRightOffset;

	switch (spriteState.verticalFacing_)
	{
	case HORIZONTAL:
		tileY += kHorizontalOffset;
		break;
	case UP:
		tileY += kUpOffset;
		break;
	case DOWN:
		tileY += kDownOffset;
		break;
	case LAST_VERTICAL_FACING:
		break;
	}

	spriteMap_[spriteState] = new Sprite(graphics, kSpritePath,
		kPolarStarIndex * kGunWidth, tileY,
		kGunWidth, kGunHeight);
}

PolarStar::Projectile::Projectile(
	Sprite* sprite,
	HorizontalFacing horizontalDir,
	VerticalFacing verticalDir,
	Vector2 position) :
	sprite_(sprite),
	horizontalDir_(horizontalDir),
	verticalDir_(verticalDir),
	position_(position),
	offset_(0)
{

}

bool PolarStar::Projectile::update(int elapsedTime)
{
	offset_ += kProjectileSpeed * elapsedTime;
	return offset_ < kProjectileMaxOffset;
}

void PolarStar::Projectile::draw(Graphics &graphics)
{
	Vector2 pos;
	pos = position_;
	pos.x = (int)round(pos.x);
	pos.y = (int)round(pos.y);

	switch (verticalDir_)
	{
	case HORIZONTAL:
		pos.x += horizontalDir_ == LEFT ? -offset_ : offset_;
		break;
	case UP:
		pos.y -= offset_;
		break;
	case DOWN:
		pos.y += offset_;
		break;
	default:
		break;
	}

	sprite_->draw(graphics, pos.x, pos.y);
}