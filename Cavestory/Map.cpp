#include "Map.h"

namespace
{
	// Sprites
	std::string kSpriteFilePath("../content/PrtCave.bmp");
}

// static
Map* Map::createTestMap(Graphics &graphics)
{
	Map* map = new Map();

	const int numRows = 15; // 15 * 32(tilesize) = 480
	const int numCols = 20;	// 20 * 32(tilesize) = 640

 	map->tiles_ = std::vector<std::vector<Tile>>(
		numRows, std::vector<Tile>(numCols));
	
	Sprite* sprite = new Sprite(graphics, kSpriteFilePath,
		Game::kTileSize, 0,
		Game::kTileSize, Game::kTileSize);

	Tile tile = Tile(sprite, WALL_TILE);

	const int row = 11;

	for (int col = 0; col < numCols; ++col)
	{
		map->tiles_[row][col] = tile;
	}

	map->tiles_[10][5] = tile;
	map->tiles_[9][4] = tile;
	map->tiles_[8][3] = tile;
	map->tiles_[7][2] = tile;
	map->tiles_[10][3] = tile;
	/*for (int i = 0; i < 11; ++i)
	{
		map->foregroundSprites_[i][0] = sprite;
		map->foregroundSprites_[i][19] = sprite;
		
	}
	for (int i = 0; i < 20; ++i)
	{
		map->foregroundSprites_[0][i] = sprite;
	}*/
	
	return map;
}

std::vector<Map::CollisionTile> Map::getCollidingTiles(const Rectangle &rectangle) const
{
	const int firstRow = rectangle.top() / Game::kTileSize;
	const int lastRow = rectangle.bottom() / Game::kTileSize;
	const int firstCol = rectangle.left() / Game::kTileSize;
	const int lastCol = rectangle.right() / Game::kTileSize;

	std::vector<CollisionTile> collisionTiles;

	for (int row = firstRow; row <= lastRow; ++row)
	{
		for (int col = firstCol; col <= lastCol; ++col)
		{
			collisionTiles.push_back(CollisionTile(row, col, tiles_[row][col].tileType_));
		}
	}

	return collisionTiles;
}

void Map::update(int elapsedTimeMs)
{
	for (size_t row = 0; row < tiles_.size(); ++row)
	{
		for (size_t col = 0; col < tiles_[row].size(); ++col)
		{
			if (tiles_[row][col].sprite_)
			{
				tiles_[row][col].sprite_->update(elapsedTimeMs);
			}
		}
	}
}

void Map::draw(Graphics &graphics) const
{
	for (size_t row = 0; row < tiles_.size(); ++row)
	{
		for (size_t col = 0; col < tiles_[row].size(); ++col)
		{
			if (tiles_[row][col].sprite_)
			{
				tiles_[row][col].sprite_->draw(
					graphics, 
					col * Game::kTileSize, row * Game::kTileSize);
			}
		}
	}
}

Map::CollisionTile::CollisionTile(int row, int col, TileType tileType) :
	row_(row), col_(col), tileType_(tileType)
{

}
