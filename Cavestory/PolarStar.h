#ifndef POLAR_STAR_H_
#define POLAR_STAR_H_

#include "Vector2.h"
#include "SpriteState.h"

#include <map>

struct Graphics;
struct Sprite;

struct PolarStar
{
	PolarStar(Graphics &graphics);

	void updateProjectiles(int elapsedTime);
	void draw(
		Graphics &graphics, 
		HorizontalFacing horizontalFacing, 
		VerticalFacing verticalFacing,
		bool gunUp,
		Vector2 playerPosition);

	void startFire(Vector2 playerPosition, HorizontalFacing horizontalFacing, VerticalFacing verticalFacing, bool gunUp);
	void stopFire();

private:
	struct SpriteState
	{
		SpriteState(
			HorizontalFacing horizontalFacing,
			VerticalFacing verticalFacing) :
			horizontalFacing_(horizontalFacing),
			verticalFacing_(verticalFacing) {}

		HorizontalFacing horizontalFacing_;
		VerticalFacing verticalFacing_;
	};

	struct Projectile
	{
		Projectile(Sprite* sprite,
			HorizontalFacing horizontalDir,
			VerticalFacing verticalDir,
			Vector2 position);


		// Returns true if this is alive
		bool update(int elapsedTime);
		void draw(Graphics &graphics);

	private:
		Sprite* sprite_;
		HorizontalFacing horizontalDir_;
		VerticalFacing verticalDir_;
		Vector2 position_;
		int offset_;
	};

	friend bool operator<(const SpriteState &a, const SpriteState &b);

	int gunX(HorizontalFacing horizontalFacing, int playerX) const;
	int gunY(VerticalFacing verticalFacing, bool gunUp, int playerY) const;

	void initializeSprites(Graphics &graphics);
	void initializeSprite(Graphics &graphics, const SpriteState &spriteState);

	std::map<SpriteState, Sprite*> spriteMap_;
	Sprite* horizontalProjectile_;
	Sprite* verticalProjectile_;

	Projectile* projectileA_;
	Projectile* projectileB_;
};

#endif