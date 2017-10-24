#include "Player.h"
#include "Graphics.h"

#include <string>

namespace
{
	// HUD Constants
	const int kHealthBarX = Game::kTileSize;
	const int kHealthBarY = 2 * Game::kTileSize;
	const int kHealthBarSourceX = 0;
	const int kHealthBarSourceY = 5 * Game::kHalfTile;
	const int kHealthBarSourceWidth = 4 * Game::kTileSize;
	const int kHealthBarSourceHeight = Game::kHalfTile;

	const int kHealthFillX = 5 * Game::kHalfTile;
	const int kHealthFillY = 2 * Game::kTileSize;
	const int kMaxFillWidth = 5 * Game::kHalfTile - 2;
	const int kHealthFillSourceX = 0;
	const int kHealthFillSourceY = 3 * Game::kHalfTile;
	const int kHealthFillSourceHeight = Game::kHalfTile;

	const int kDamageSourceX = 0;
	const int kDamageSourceY = 2 * Game::kTileSize;
	const int kDamageSourceHeight = Game::kHalfTile;

	const int kHealthNumberX = 3 * Game::kHalfTile;
	const int kHealthNumberY = 2 * Game::kTileSize;
	const int kHealthNumberNumDigits = 2;

	const std::string kSpritePath = "../content/TextBox.bmp";
	const int kDamageDelay = 1500; // ms
}

Player::Health::Health(Graphics &graphics) :
	damage_(0),
	damageTime_(0),
	maxHealth_(6),
	currentHealth_(6),
	healthBarSprite_(Sprite(graphics, kSpritePath,
		kHealthBarSourceX, kHealthBarSourceY,
		kHealthBarSourceWidth, kHealthBarSourceHeight)),
	healthFillSprite_(VaryingWidthSprite(graphics, kSpritePath,
		kHealthFillSourceX, kHealthFillSourceY,
		kMaxFillWidth, kHealthFillSourceHeight)),
	damageFillSprite_(VaryingWidthSprite(graphics, kSpritePath,
		kDamageSourceX, kDamageSourceY,
		0, kDamageSourceHeight))
{

}
void Player::Health::update(int elapsedTime)
{
	if (damage_ > 0)
	{
		damageTime_ += elapsedTime;
		if (damageTime_ > kDamageDelay)
		{
			currentHealth_ -= damage_;
			damage_ = 0;
		}
	}
}

void Player::Health::draw(Graphics &graphics)
{
	healthBarSprite_.draw(graphics, kHealthBarX, kHealthBarY);
	healthFillSprite_.draw(graphics, kHealthFillX, kHealthFillY);

	if (damage_ > 0)
	{
		damageFillSprite_.draw(graphics, kHealthFillX + fillOffset(currentHealth_ - damage_), kHealthFillY);
	}

	NumberSprite(graphics, currentHealth_, kHealthNumberNumDigits).draw(graphics, kHealthNumberX, kHealthNumberY);
}

bool Player::Health::takeDamage(int damage)
{
	damage_ = damage;
	damageTime_ = 0;
	healthFillSprite_.setWidth(fillOffset(currentHealth_ - damage));
	damageFillSprite_.setWidth(fillOffset(damage));

	return currentHealth_ < maxHealth_;
}

int Player::Health::fillOffset(int health) const
{
	return kMaxFillWidth * health / maxHealth_;
}