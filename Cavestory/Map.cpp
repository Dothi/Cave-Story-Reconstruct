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

	map->backdrop_ = new FixedBackdrop("../content/bkBlue.bmp", graphics);

	const int numRows = 24; // 15 * 32(tilesize) = 480
	const int numCols = 32;	// 20 * 32(tilesize) = 640

 	map->tiles_ = std::vector<std::vector<Tile>>(
		numRows, std::vector<Tile>(numCols));

	map->backgroundTiles_ = std::vector<std::vector<Sprite*>>(
		numRows, std::vector<Sprite*>(numCols));
	
	Sprite* sprite = new Sprite(graphics, kSpriteFilePath,
		Game::kTileSize, 0,
		Game::kTileSize, Game::kTileSize);

	Tile tile = Tile(sprite, WALL_TILE);

	const int lastRow = numRows - 1;

	for (int col = 0; col < numCols; ++col)
	{
		map->tiles_[lastRow][col] = tile;
	}
	const int lastCol = numCols - 1;

	for (int row = 0; row < numRows; ++row)
	{
		map->tiles_[row][0] = tile;
		map->tiles_[row][lastCol] = tile;
	}

	const int firstRow = 0;

	for (int col = 0; col < numCols; ++col)
	{
		map->tiles_[firstRow][col] = tile;
	}
	map->tiles_[lastRow - 4][5] = tile;
	map->tiles_[lastRow - 4][6] = tile;
	map->tiles_[lastRow - 5][6] = tile;
	map->tiles_[lastRow - 4][4] = tile;
	map->tiles_[lastRow - 4][27] = tile;
	map->tiles_[lastRow - 4][26] = tile;
	map->tiles_[lastRow - 4][25] = tile;
	map->tiles_[lastRow - 5 ][25] = tile;
	map->tiles_[lastRow - 1][30] = tile;
	map->tiles_[lastRow - 1][1] = tile;
	//map->tiles_[lastRow - 3][27] = tile;
	for (int col = 6; col < numCols - 6; ++col)
	{
		map->tiles_[lastRow - 6][col] = tile;
	}

	Sprite* chainTop = new Sprite(graphics, kSpriteFilePath,
		11 * Game::kTileSize, 2 * Game::kTileSize,
		Game::kTileSize, Game::kTileSize);

	Sprite* chainMiddle = new Sprite(graphics, kSpriteFilePath,
		12 * Game::kTileSize, 2 * Game::kTileSize,
		Game::kTileSize, Game::kTileSize);

	Sprite* chainBottom = new Sprite(graphics, kSpriteFilePath,
		13 * Game::kTileSize, 2 * Game::kTileSize,
		Game::kTileSize, Game::kTileSize);

	map->backgroundTiles_[18][9] = chainTop;
	map->backgroundTiles_[19][9] = chainBottom;
	map->backgroundTiles_[18][22] = chainTop;
	map->backgroundTiles_[19][22] = chainBottom;

	for (int col = 9; col < numCols - 9; ++col)
	{
		map->tiles_[lastRow - 3][col] = tile;
	}

	
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

void Map::drawBackground(Graphics &graphics) const
{
	backdrop_->draw(graphics);

	for (size_t row = 0; row < backgroundTiles_.size(); ++row)
	{
		for (size_t col = 0; col < backgroundTiles_[row].size(); ++col)
		{
			if (backgroundTiles_[row][col])
			{
				backgroundTiles_[row][col] ->draw(
					graphics,
					col * Game::kTileSize, row * Game::kTileSize);
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
