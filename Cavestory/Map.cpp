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

	map->foregroundSprites_ = std::vector<std::vector<Sprite*>>(
		numRows, std::vector<Sprite*>(numCols));
	
	Sprite* sprite = new Sprite(graphics, kSpriteFilePath,
		Game::kTileSize, 0,
		Game::kTileSize, Game::kTileSize);


	const int row = 11;

	for (int col = 0; col < numCols; ++col)
	{
		map->foregroundSprites_[row][col] = sprite;
	}

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

void Map::update(int elapsedTimeMs)
{
	for (size_t row = 0; row < foregroundSprites_.size(); ++row)
	{
		for (size_t col = 0; col < foregroundSprites_[row].size(); ++col)
		{
			if (foregroundSprites_[row][col])
			{
				foregroundSprites_[row][col]->update(elapsedTimeMs);
			}
		}
	}
}

void Map::draw(Graphics &graphics) const
{
	for (size_t row = 0; row < foregroundSprites_.size(); ++row)
	{
		for (size_t col = 0; col < foregroundSprites_[row].size(); ++col)
		{
			if (foregroundSprites_[row][col])
			{
				foregroundSprites_[row][col]->draw(
					graphics, 
					col * Game::kTileSize, row * Game::kTileSize);
			}
		}
	}
}