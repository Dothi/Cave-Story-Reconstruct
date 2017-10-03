#ifndef GAME_H_
#define GAME_H_

#include <SDL.h>
#include "Graphics.h"
#include "Input.h"
#include "Player.h"
#include "Map.h"

struct Player;
struct Map;

struct Game
{
	Game();
	~Game();

	static int kTileSize;
private:
	void eventLoop();
	void update(int elapsedTimeMs);
	void draw(Graphics &graphics);
	void handleInput(Input &input);

	Player *player_;
	Map *map_;

	bool running;
	
};

#endif // GAME_H_