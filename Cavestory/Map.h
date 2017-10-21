#ifndef MAP_H_
#define MAP_H_

#include <vector>
#include <string>
#include "Sprite.h"
#include "Graphics.h"
#include "Game.h"
#include "Rectangle.h"
#include "Backdrop.h"


struct Graphics;
struct Sprite;


struct Map
{
	enum TileType
	{
		AIR_TILE,
		WALL_TILE
	};

	struct CollisionTile
	{
		CollisionTile(int row, int col, TileType tileType);

		int row_, col_;
		TileType tileType_;

	};

	static Map *createTestMap(Graphics &graphics);

	std::vector<CollisionTile> getCollidingTiles(const Rectangle &rectangle) const;

	void update(int elapsedTimeMs);
	void drawBackground(Graphics &graphics) const;
	void draw(Graphics &graphics) const;

private:


	struct Tile
	{
		Tile() : sprite_(nullptr), tileType_(AIR_TILE) {}
		Tile(Sprite *sprite,
			TileType tileType = AIR_TILE) :
			tileType_(tileType), sprite_(sprite) {}

		TileType tileType_;
		Sprite *sprite_;
	};
	Backdrop *backdrop_;
	std::vector<std::vector<Sprite*>> backgroundTiles_;
	std::vector<std::vector<Tile>> tiles_;
};

#endif
