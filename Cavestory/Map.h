#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <string>
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"


struct Graphics;
struct Sprite;

struct Map
{
	static Map* createTestMap(Graphics &graphics);

	void update(int elapsedTimeMs);
	void draw(Graphics &graphics) const;

private:
	std::vector<std::vector<Sprite*>> foregroundSprites_;
};

#endif
